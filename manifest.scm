;; guix shell --container --emulate-fhs --network --preserve='^DISPLAY$' -m manifest.scm
(specifications->manifest
 '("bash"
   "python"
   "coreutils"
   "make"
   "gcc:lib"
   "zlib"
   "pkg-config"))
