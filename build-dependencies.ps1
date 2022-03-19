param(
	[ValidateSet("Release", "Debug")][String[]]$BuildConfigurations="Release,Debug"
)

foreach ($BuildConfiguration in $BuildConfigurations) {
	conan install $PSScriptRoot/build/conan --profile $PSScriptRoot/build/conan/profiles/$BuildConfiguration-x64 --install-folder $PSScriptRoot/build/conan/generated/$BuildConfiguration-x64 --build=missing
}
