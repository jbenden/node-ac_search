#include <node.h>
#include <v8.h>
#include <string.h>

#include "AhoCorasick.h"

using namespace v8;
using namespace node;

const char *
ToCString(const v8::String::Utf8Value &value) {
    return *value;
}

class Ac : public node::ObjectWrap
{
    public:
        Ac() {};
        virtual ~Ac() {};

        static void Init(Handle<Object> target);

    private:
        static Handle<Value> acNew(const Arguments& args);
        static Handle<Value> acAdd(const Arguments& args);
        static Handle<Value> acCompile(const Arguments& args);
        static Handle<Value> acSearch(const Arguments& args);

        // aho_corasick_t root;
        AhoCorasick root;
};

Handle<Value> Ac::acNew(const Arguments& args) {
    HandleScope scope;

    if (!args.IsConstructCall()) 
		return ThrowException(String::New("Cannot call constructor as function"));
    
    Ac *ac = new Ac();
    
    ac->Wrap(args.This());

    return args.This();
}
 
void Ac::Init(Handle<Object> target) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(acNew);
    tpl->SetClassName(String::NewSymbol("ac_search"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // prototype functions
    tpl->PrototypeTemplate()->Set(String::NewSymbol("add"),
            FunctionTemplate::New(acAdd)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("compile"),
            FunctionTemplate::New(acCompile)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("search"),
            FunctionTemplate::New(acSearch)->GetFunction());

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol("ac_search"), constructor);
}

Handle<Value> Ac::acAdd(const Arguments &args) {
    HandleScope scope;

    if (args.Length() < 1) {
        ThrowException(Exception::TypeError(String::New("Wrong number of parameters.")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString()) {
        return ThrowException(Exception::TypeError(String::New("Second argument must be a string.")));
    }

    v8::String::Utf8Value str(args[0]);
    const char *aStr = ToCString(str);
    Ac *ac = ObjectWrap::Unwrap<Ac>(args.This());

    Local<Number> num = Number::New(ac->root.add(aStr, strlen(aStr)));

    return scope.Close(num);
}

Handle<Value> Ac::acCompile(const Arguments &args) {
    HandleScope scope;

    Ac *ac = ObjectWrap::Unwrap<Ac>(args.This());

    Local<Number> num = Number::New(ac->root.compile());
    return scope.Close(num);
}

Handle<Value> Ac::acSearch(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Second argument must be a string.")));
        return scope.Close(Undefined());
    }

    Ac *ac = ObjectWrap::Unwrap<Ac>(args.This());
    
    v8::String::Utf8Value str(args[0]);
    const char *aStr = ToCString(str);
    
    int ret;
    Local<v8::String> value;
    char match[512];

    ret = ac->root.search(aStr, strlen(aStr), (char *) match);
    if (ret) {
        value = v8::String::New(match);
    }

    return scope.Close(value);
}

NODE_MODULE(ac_search, Ac::Init)

