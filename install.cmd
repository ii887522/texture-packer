python -m pip install virtualenv
virtualenv env
env\Scripts\activate && python -m pip install cpplint && env\Scripts\deactivate && docker run --rm --name texture-packer_installer -w C:\texture-packer -v %CD%:C:\texture-packer texture-packer-custom-node ncu.cmd -u && npm.cmd install
