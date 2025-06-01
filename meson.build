project(
    'widgets',
    'cpp',
    default_options: [
        'cpp_std=c++23',
        'c_args=-fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O0 -Wall -Wextra',
        'cpp_args=-fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O0 -Wall -Wextra',
        'c_link_args=-fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O0 -Wall -Wextra',
        'cpp_link_args=-fsanitize=address,undefined -fno-omit-frame-pointer -g3 -O0 -Wall -Wextra'
    ],
    meson_version: '>1.5.0'
)

qt6 = import('qt6')

subdir('Opal')
subdir('Config')