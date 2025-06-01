config_headers = files(
    'include/Config/Widgets/Bar/bar.hpp',

    'include/Config/private/widgets/settingsmenu/settings_menu_p.hpp',
    'include/Config/Widgets/SettingsMenu/settings_menu.hpp',
)

config_sources = files(
    'src/widgets/bar/bar.cpp',

    'src/widgets/settings_menu/settings_menu.cpp',

    'src/main.cpp',
)

config_compiled_moc = qt6.compile_moc(
    headers: config_headers,
    preserve_paths: true
)

config_include = include_directories('include')

executable(
    'opal_widgets',
    sources: [
        config_sources,
        config_compiled_moc
    ],
    include_directories: [
        opal_core_include,
        opal_util_sass_embedded_include,
        opal_widgets_include,
        config_include
    ],
    dependencies: [
        dependency('qt6', modules: [ 'Core', 'Gui', 'Widgets' ]),
        dependency('LayerShellQt', modules: [ 'LayerShellQt::Interface' ])
    ],
    link_with: [
        opal_core_lib,
        opal_util_sass_embedded_lib,
        opal_widgets_lib
    ]
)
