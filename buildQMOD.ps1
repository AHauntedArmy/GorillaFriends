./build.ps1

$ArchiveName = "gorillafriends_v1.0.1.qmod"
$TempArchiveName = "gorillafriends.zip"

Compress-Archive -Path  "./libs/arm64-v8a/libgorillafriends.so", 
                        "./libs/arm64-v8a/libbeatsaber-hook_2_3_0.so", 
                        "./libs/arm64-v8a/libcustom-types.so",
                        "./kittyfriend.png",
                        "./mod.json" -DestinationPath $TempArchiveName -Force
                        
Move-Item $TempArchiveName $ArchiveName -Force