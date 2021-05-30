docker run --rm --name texture-packer_deployer -w C:\texture-packer -v %CD%:C:\texture-packer texture-packer-custom-node node deploy.js %1 %2
