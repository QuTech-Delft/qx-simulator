import os
from setuptools import setup
from shutil import copyfile
import subprocess
from sys import platform

rootDir = os.path.dirname(os.path.realpath(__file__))
buildDir = os.path.join(rootDir, "cbuild")
libqasmBuildDir = os.path.join(rootDir, "libqasmbuild")
clibDir = buildDir

if not os.path.exists(buildDir):
    os.makedirs(buildDir)

if not os.path.exists(libqasmBuildDir):
    os.makedirs(libqasmBuildDir)

if platform == "linux" or platform == "linux2":
    print('Detected Linux OS, installing qxelarator ... ')
    os.chdir(libqasmBuildDir)
    cmd = 'cmake ../..'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'make'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()

    os.chdir(buildDir)
    cmd = 'cmake ../qxelarator/.'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'make'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    clibname = "_qxelarator.so"

elif platform == "darwin":
    print('Detected OSX, installing qxelarator ... ')
    os.chdir(libqasmBuildDir)
    cmd = 'cmake ../..'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'make'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()

    os.chdir(buildDir)
    cmd = 'cmake ../qxelarator/.'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    cmd = 'make'
    proc = subprocess.Popen(cmd, shell=True)
    proc.communicate()
    clibname = "_qxelarator.so"

else:
    print('Unknown/Unsupported OS !!!')

genclib = os.path.join(clibDir, clibname)
clib = os.path.join(rootDir, "qxelarator", clibname)
copyfile(genclib, clib)

copyfile(os.path.join(clibDir, "qxelarator.py"),
         os.path.join(rootDir, "qxelarator", "qxelarator.py"))

os.chdir(rootDir)

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

try:
    from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
    class bdist_wheel(_bdist_wheel):
        def finalize_options(self):
            _bdist_wheel.finalize_options(self)
            self.root_is_pure = False
except ImportError:
    bdist_wheel = None


setup(name='qxelarator',
      version='0.4.0',
      description='qxelarator Python Package',
      # long_description=read('README.md'),
      author='Imran Ashraf',
      author_email='iimran.aashraf@gmail.com',
      url="https://github.com/QE-Lab/qx-simulator",
      packages=['qxelarator'],
      cmdclass={'bdist_wheel': bdist_wheel},
      include_package_data=True,
      package_data={'qxelarator': [clib]},
      zip_safe=False)

