env\Scripts\activate && cpplint --recursive --linelength=256 texture-packer\src\ && env\Scripts\deactivate && docker run --rm --name texture-packer_test_x86_builder -w C:\texture-packer -v %CD%:C:\texture-packer ms-build-tools msbuild C:\texture-packer\texture-packer\texture-packer.sln /p:Configuration=Test /p:Platform=x86 && docker run --rm --name texture-packer_test_x64_builder -w C:\texture-packer -v %CD%:C:\texture-packer ms-build-tools msbuild C:\texture-packer\texture-packer\texture-packer.sln /p:Configuration=Test /p:Platform=x64 && cd texture-packer && Test\texture-packer.exe && x64\Test\texture-packer.exe && cd .. && docker run --rm --name texture-packer_debug_x86_builder -w C:\texture-packer -v %CD%:C:\texture-packer ms-build-tools msbuild C:\texture-packer\texture-packer\texture-packer.sln /p:Configuration=Debug /p:Platform=x86 && docker run --rm --name texture-packer_debug_x64_builder -w C:\texture-packer -v %CD%:C:\texture-packer ms-build-tools msbuild C:\texture-packer\texture-packer\texture-packer.sln /p:Configuration=Debug /p:Platform=x64 && docker run --rm --name texture-packer_release_x86_builder -w C:\texture-packer -v %CD%:C:\texture-packer ms-build-tools msbuild C:\texture-packer\texture-packer\texture-packer.sln /p:Configuration=Release /p:Platform=x86 && docker run --rm --name texture-packer_release_x64_builder -w C:\texture-packer -v %CD%:C:\texture-packer ms-build-tools msbuild C:\texture-packer\texture-packer\texture-packer.sln /p:Configuration=Release /p:Platform=x64
