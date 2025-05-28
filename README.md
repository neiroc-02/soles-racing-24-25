# SOLES RACING PROJECT CODE
## Description
This is the repository for all the code for SOLES Racing, regarding the Arduino Code and the GUI code.

## Installation

### Windows
Run commands through MSYS2 MINGW64 shell (make sure you are in the GUI folder)

```
pacman -S --noconfirm mingw-w64-x86_64-gcc
pacman -S --noconfirm mingw-w64-x86_64-make
pacman -S --noconfirm mingw-w64-x86_64-pkg-config
pacman -S --noconfirm mingw-w64-x86_64-glfw
```

```
make
```

```
./soles-racing
```

### Linux 
#### Setting the GUI up:
1. Download all dependencies: `./some_installs.sh`
2. Then create the executable: `make`
3. Then run the executable: `./soles_racing`

#### Setting up the mosquitto server:
1. Install mosquitto dependencies (listed in `./some_installs.sh`)
2. Create a mosquitto.conf file: `sudo vim /etc/mosquitto/mosquitto.conf`
3. Build the mosquitto.conf file as follows:
```
pid_file /run/mosquitto/mosquitto.pid

persistence true
persistence_location /var/lib/mosquitto/

log_dest topic

log_type error
log_type warning
log_type notice
log_type information

bind_address 0.0.0.0

connection_messages true
log_timestamp true
allow_anonymous true
listener 1883

include_dir /etc/mosquitto/conf.d
```
4. Start the mosquitto server with the following command: `mosquitto -v -c /etc/mosquitto/mosquitto.conf`

### Mac
TODO

## Usage 

## Windows
Running the following command shows the installed packages in the current environment: 
```
pacman -Q
```
If you are having trouble running the GUI, ensure you have the following packages installed:
```
base 2022.06-1
bash 5.2.037-2
bash-completion 2.16.0-1
brotli 1.1.0-1
bsdtar 3.7.7-1
bzip2 1.0.8-4
ca-certificates 20241223-1
coreutils 8.32-5
curl 8.12.1-1
dash 0.5.12-1
db 6.2.32-5
dos2unix 7.5.2-1
file 5.46-1
filesystem 2025.02.23-1
findutils 4.10.0-2
gawk 5.3.1-1
gcc-libs 13.3.0-1
gdbm 1.24-1
getent 2.18.90-5
gettext 0.22.5-1
gmp 6.3.0-1
gnupg 2.4.7-1
grep 1~3.0-7
gzip 1.13-1
heimdal-libs 7.8.0-5
inetutils 2.5-2
info 7.1.1-1
less 668-1
libargp 20241207-1
libasprintf 0.22.5-1
libassuan 2.5.7-1
libbz2 1.0.8-4
libcurl 8.12.1-1
libdb 6.2.32-5
libedit 20240808_3.1-1
libexpat 2.6.4-1
libffi 3.4.7-1
libgcrypt 1.11.0-1
libgdbm 1.24-1
libgettextpo 0.22.5-1
libgnutls 3.8.9-1
libgpg-error 1.51-1
libhogweed 3.10.1-1
libiconv 1.18-1
libidn2 2.3.7-1
libintl 0.22.5-1
libksba 1.6.7-1
liblz4 1.10.0-1
liblzma 5.6.4-1
libnettle 3.10.1-1
libnghttp2 1.64.0-1
libnpth 1.8-1
libopenssl 3.4.1-1
libp11-kit 0.25.5-2
libpcre 8.45-5
libpcre2_8 10.45-1
libpsl 0.21.5-2
libreadline 8.2.013-1
libsqlite 3.46.1-1
libssh2 1.11.1-1
libtasn1 4.20.0-1
libunistring 1.3-1
libutil-linux 2.40.2-2
libxcrypt 4.4.38-1
libzstd 1.5.7-1
make 4.4.1-2
mingw-w64-x86_64-binutils 2.44-1
mingw-w64-x86_64-bzip2 1.0.8-3
mingw-w64-x86_64-crt-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-expat 2.6.4-1
mingw-w64-x86_64-gcc 14.2.0-2
mingw-w64-x86_64-gcc-libs 14.2.0-2
mingw-w64-x86_64-gdb 16.2-1
mingw-w64-x86_64-gdb-multiarch 16.2-1
mingw-w64-x86_64-gettext-runtime 0.23.1-1
mingw-w64-x86_64-glew 2.2.0-3
mingw-w64-x86_64-glfw 3.4-1
mingw-w64-x86_64-gmp 6.3.0-2
mingw-w64-x86_64-headers-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-isl 0.27-1
mingw-w64-x86_64-libffi 3.4.7-1
mingw-w64-x86_64-libiconv 1.18-1
mingw-w64-x86_64-libmangle-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-libsystre 1.0.1-6
mingw-w64-x86_64-libtre 0.9.0-1
mingw-w64-x86_64-libwinpthread-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-make 4.4.1-3
mingw-w64-x86_64-mpc 1.3.1-2
mingw-w64-x86_64-mpdecimal 4.0.0-1
mingw-w64-x86_64-mpfr 4.2.1-2
mingw-w64-x86_64-ncurses 6.5.20241228-3
mingw-w64-x86_64-openssl 3.4.1-1
mingw-w64-x86_64-pkgconf 1~2.3.0-1
mingw-w64-x86_64-python 3.12.9-3
mingw-w64-x86_64-readline 8.2.013-1
mingw-w64-x86_64-sqlite3 3.47.2-1
mingw-w64-x86_64-tcl 8.6.13-1
mingw-w64-x86_64-termcap 1.3.1-7
mingw-w64-x86_64-tk 8.6.13-1
mingw-w64-x86_64-tools-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-tzdata 2025a-1
mingw-w64-x86_64-windows-default-manifest 6.4-4
mingw-w64-x86_64-winpthreads-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-winstorecompat-git 12.0.0.r509.g079e6092b-1
mingw-w64-x86_64-xxhash 0.8.3-1
mingw-w64-x86_64-xz 5.6.4-1
mingw-w64-x86_64-zlib 1.3.1-1
mingw-w64-x86_64-zstd 1.5.7-1
mintty 1~3.7.7-2
mpfr 4.2.1-1
msys2-keyring 1~20250214-1
msys2-launcher 1.5-3
msys2-runtime 3.5.7-2
nano 8.3-1
ncurses 6.5.20240831-2
nettle 3.10.1-1
openssl 3.4.1-1
p11-kit 0.25.5-2
pacman 6.1.0-11
pacman-contrib 1.10.6-1
pacman-mirrors 20250220-1
perl 5.38.2-3
pinentry 1.3.1-1
rebase 4.5.0-4
sed 4.9-1
tar 1.35-2
time 1.9-3
tzcode 2025a-1
util-linux 2.40.2-2
wget 1.25.0-1
which 2.21-4
xz 5.6.4-1
zlib 1.3.1-1
zstd 1.5.7-1
```