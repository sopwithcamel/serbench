import os.path

env = Environment()

src_files = [Glob('*.cpp'), Glob('*.cc')]

env.Append(CCFLAGS = ['-g', '-O3', '-Wall'])
testapp = env.Program('./test', src_files,
            LIBS = ['-lprotobuf', '-lmsgpack'])
## Targets
# build targets
build = env.Alias('build', [testapp])
env.Default(*build)
