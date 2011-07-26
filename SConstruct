env = Environment()

build_examples = ARGUMENTS.get('BUILD_EXAMPLES', '1')
install_include = ARGUMENTS.get('INSTALL_INCLUDE', '1')
debug = ARGUMENTS.get('DEBUG', '0')
cflags = ARGUMENTS.get('CFLAGS', '')
destdir = ARGUMENTS.get('DESTDIR', '/usr/local/')
oss_include_dir = ARGUMENTS.get('OSS_INCLUDE_DIR', '/usr/local/lib/oss/include')

cflags += ' '
if debug == '1':
    cflags += '-g -DDEBUG '
cflags += '-Iinclude -I' + oss_include_dir + ' '

Export('env', 'debug', 'cflags', 'destdir', 'oss_include_dir')

SConscript('src/SConscript')

if install_include == '1':
    SConscript('include/SConscript')

if build_examples == '1':
    SConscript('examples/SConscript')

libdatadir = destdir + '/libdata/pkgconfig/'
env.Install(libdatadir, 'mix.pc')
env.Alias('install', libdatadir)
