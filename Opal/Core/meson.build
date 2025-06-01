qt6 = import('qt6')

# TODO: figure out a better layout, and change privates
opal_core_headers = files(
    'include/Opal/Core/Settings/settings.hpp',
    
    'include/Opal/Core/private/application_p.hpp',
    'include/Opal/Core/application.hpp'
)

opal_core_sources = files(
    'src/settings/settings.cpp',
    'src/application.cpp'
)

opal_core_compiled_moc = qt6.compile_moc(
    headers: opal_core_headers,
    preserve_paths: true
)

opal_core_include = include_directories('include')

opal_core_lib = static_library(
    'opal_core',
    sources: [
        opal_core_sources,
        opal_core_compiled_moc
    ],
    include_directories: [
        opal_util_sass_embedded_include,
        opal_core_include
    ],
    dependencies: [
        dependency('qt6', modules: [ 'Core', 'Gui', 'Widgets' ])
    ],
    link_with: [
        opal_util_sass_embedded_lib
    ]
)