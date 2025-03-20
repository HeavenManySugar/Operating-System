# Kernel Modules: Jiffies and Seconds

This project contains two Linux kernel modules:
1. `jiffies`: Exposes the current value of `jiffies` through `/proc/jiffies`.
2. `seconds`: Exposes the number of seconds since the module was loaded through `/proc/seconds`.

## Building the Modules

To build the modules, run the following command in the project directory:

```bash
make
```

This will compile the `jiffies.c` and `seconds.c` files into kernel modules.

## Cleaning Up

To clean up the build artifacts, run:

```bash
make clean
```

## Using `clangd` for Development

If you want to use `clangd` for code completion and navigation, you need to generate a `compile_commands.json` file. This can be done using `bear`:

1. Install `bear` if it is not already installed:
   ```bash
   sudo apt install bear
   ```

2. Run the following command to generate the `compile_commands.json` file:
   ```bash
   bear -- make
   ```

3. Open the project in your editor, and `clangd` will use the `compile_commands.json` file for better code analysis.

## Loading and Testing the Modules

### Jiffies Module

1. Load the `jiffies` module:
   ```bash
   sudo insmod jiffies.ko
   ```

2. Check the `/proc/jiffies` file:
   ```bash
   cat /proc/jiffies
   ```

3. Unload the module:
   ```bash
   sudo rmmod jiffies
   ```

4. Verify that the module is removed:
   ```bash
   lsmod | grep jiffies
   ```

### Seconds Module

1. Load the `seconds` module:
   ```bash
   sudo insmod seconds.ko
   ```

2. Check the `/proc/seconds` file:
   ```bash
   cat /proc/seconds
   ```

3. Unload the module:
   ```bash
   sudo rmmod seconds
   ```

4. Verify that the module is removed:
   ```bash
   lsmod | grep seconds
   ```
