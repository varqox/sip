project('sip',
    ['c', 'cpp'],
    license: 'MIT',
    meson_version : '>=0.52.1',
    default_options : meson.get_cross_property('project_configuration', [
        'cpp_std=c++17',
        'warning_level=3',
    ])
)

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

if get_option('warning_level') > '0'
    warnings = [
        '-Wshadow',
        '-Wunreachable-code',
        '-Wdocumentation',
        '-Wgnu',
        '-Wunused-lambda-capture',
    ]
    foreach warning : warnings
        if cc.has_argument(warning)
            add_project_arguments(warning, language : 'c')
        endif
        if cpp.has_argument(warning)
            add_project_arguments(warning, language : 'cpp')
        endif
    endforeach
endif

static_kwargs = {}
if get_option('static')
    static_kwargs = {'static': true}
    add_project_link_arguments(['-static'], language : ['c', 'cpp'])
endif

dump_file_as_c_array = generator(find_program('subprojects/simlib/dump_file_as_c_array.py'),
    output : ['@BASENAME@_dump.c'],
    arguments : ['@INPUT@', '@EXTRA_ARGS@'],
    capture : true,
)

simlib_proj = subproject('simlib')
simlib_dep = simlib_proj.get_variable('simlib_dep')

sip_sources = [
    'src/command_version.cc',
    'src/commands.cc',
    'src/compilation_cache.cc',
    'src/main.cc',
    'src/sip_package.cc',
    'src/sipfile.cc',
    'src/templates.cc',
    'src/tests_files.cc',
    'src/utils.cc',
]

if host_machine.cpu_family() == 'x86'
    proot_bin = 'bin/proot-x86'
elif host_machine.cpu_family() == 'x86_64'
    proot_bin = 'bin/proot-x86_64'
else
    error('This CPU architecture is not supported')
endif

sip = executable('sip', sources : [
        sip_sources,
        dump_file_as_c_array.process(
            proot_bin,
            extra_args : 'proot_dump'
        ),
        dump_file_as_c_array.process(
            'templates/checker.cc',
            extra_args : 'checker_cc'
        ),
        dump_file_as_c_array.process(
            'templates/interactive_checker.cc',
            extra_args : 'interactive_checker_cc'
        ),
        dump_file_as_c_array.process(
            'templates/statement.tex',
            extra_args : 'statement_tex'
        ),
        custom_target('git_commit.hh',
            command : find_program('src/gen_git_commit_hh.py'),
            depend_files : '.git/logs/HEAD',
            output : 'git_commit.hh',
            capture : true,
        ),
    ],
    dependencies : [
        simlib_dep,
    ],
    install : true,
    install_rpath : get_option('prefix') / get_option('libdir'),
)

alias_target('base', sip)
run_target('format', command : [find_program('format.py'), meson.current_source_dir()])
run_target('tidy', command : [find_program('tidy.sh')], depends : sip)