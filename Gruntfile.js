module.exports = function(grunt) {
    grunt.loadNpmTasks('grunt-mocha-test');
    grunt.loadNpmTasks('grunt-coveralls');
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        mochaTest: {
            test: {
                options: {
                    quiet: false,
                    timeout: 10000
                },
                src: ['**/*_test.js']
            }
        },
        coveralls: {
            options: {
                src: 'coverage/lcov.info',
                force: false
            }
        }
    });
    grunt.registerTask('default', ['mochaTest','coveralls']);
};
