project('ryzen_controller', ['cpp', 'c'],
  version: '0.1.0',
  default_options: ['cpp_std=c++23']
)

gtk_dep = dependency('gtk4')
libadwaita_dep = dependency('libadwaita-1')
cc = meson.get_compiler('c')

ryzenadj_src = [
    'ryzenadj/argparse.c',
    'ryzenadj/main.c',
    'ryzenadj/lib/nb_smu_ops.c',
    'ryzenadj/lib/api.c',
    'ryzenadj/lib/cpuid.c',
    'ryzenadj/lib/linux/osdep_linux.c',
    'ryzenadj/lib/linux/osdep_linux_mem.c',
    'ryzenadj/lib/linux/osdep_linux_smu_kernel_module.c'
]

# ryzenadj requires libpci
pci_dep = dependency('libpci', required : false)
if not pci_dep.found()
    pci_dep = cc.find_library('pci', required : true)
endif

ryzen_helper = executable('ryzen_helper',
    ryzenadj_src,
    c_args: ['-D_LIBRYZENADJ_INTERNAL'],
    dependencies: [pci_dep],
    install: true,
    install_dir: get_option('libexecdir') / 'kerneldrive' 
)

ryzen_sources = [
    'ryzen_plugin.cpp',
    'backend/ryzen_adj_wrapper.cpp',
    'ui/ryzen_page.cpp'
]

# Include Path to Core (Now Local)
inc = include_directories('.')

shared_library('ryzen_controller',
    ryzen_sources,
    include_directories: inc,
    dependencies: [gtk_dep, libadwaita_dep],
    install: true,
    install_dir: get_option('libdir') / 'kerneldrive/plugins',
    cpp_args: ['-DRYZEN_HELPER_PATH="' + get_option('prefix') / get_option('libexecdir') / 'kerneldrive/ryzen_helper"'],
    name_prefix: ''
)
