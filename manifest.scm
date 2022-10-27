;; $ guix shell --container --emulate-fhs --network -m manifest.scm
(specifications->manifest
 '("bash"
   "git"
   "coreutils"
   "pkg-config"

   ;; Tools needed for emsdk to operate
   "python"
   "python-certifi"
   "which"
   "tar"
   "xz"
   "lbzip2"

   ;; Libraries needed for bundled tools
   "gcc:lib"
   "zlib"))
