# OS1000

<a href="https://operating-system-in-1000-lines.vercel.app/zh/">OS in 1000 lines</a>
re-constructed as the source tutorial has some kind of un-elegant  
It turns out that this OS is not suitable for persistent use as it always runs into panic after a second boot.  
But anyway, a great toy for operatin system teaching, isn't it?  
Please feel free to fork this repo and modify it.  

after cloning the repo, run following command to download OpenSBI in the project directory:
```shell
curl -LO https://github.com/qemu/qemu/raw/v8.0.4/pc-bios/opensbi-riscv32-generic-fw_dynamic.bin
```

simply run ``make`` will generate the ``kernel.elf``  
run
```shell
make qemu
```
will boot the system using kernel.elf
