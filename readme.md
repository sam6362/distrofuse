# DistroFuse V1.0

Distrofuse is an open-source project that creates a single, cross-distribution interface for diverse Linux package managers (Snap, Flatpak, DNF, Pacman, etc.).<br>
It also significantly enhances curl into a smarter download and content management utility. 

## Features

- 🧩 **Easy to use**: Install any package from which manager you want.
- ⚡ **Fast**: Install packages from the closest mirror for the fastest downloading experience.
- 🔍 **Modern UI**: Download and Install packages within **Modern and easy to track UI**
- 📝 **Package Managers**: You can currently use **Flatpak, dnf, pacman** and **apt**.
- 🛡️ **Active Development**: This project will be developed for **widely used** package managers like **aur, yum, snap, etc.** and Customization.

## Prerequisites

- GCC Compiler
- Any of the **Package Managers**
- CMake and Make

## Installation

1. Cloning the Repository

```bash
git clone git@github.com:trycatchh/distrofuse.git
cd distfuse
```

2. Building

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
sudo make install
```

3. Checking

```bash
distfuse --version
```

## Using

Installing any package.<br>
`distfuse install package-name`

Removing any package.<br>
`distfuse remove package-name`

Sometimes you will need to use **Sudo**

## Examples
Sometimes you will need to use **Sudo**

Installing any package.<br>
`sudo distfuse install btop`

Removing any package.<br>
`sudo distfuse remove btop`

## Active Team

<div align="center">
<table>
  <tr>
    <td align="center" width="200">
      <a href="https://github.com/Mal1koRe1ss">
        <img src="https://avatars.githubusercontent.com/u/70070751?v=4" height="150" style="border-radius: 50%"><br>
        @Mal1koRe1ss
      </a>
    </td>
    <td align="center" width="200">
      <a href="https://github.com/p0unter">
        <img src="https://avatars.githubusercontent.com/u/138019503?v=4" height="150" style="border-radius: 50%"><br>
        @p0unter
      </a>
    </td>
  </tr>
</table>
</div>

## License

This project is licensed under the APACHE License - see the [LICENSE](LICENSE) file for details.

## Support

For support or feature requests:
- [Create an **Issue**](https://github.com/Mal1kore1ss/V14Template/issues)
- [Star the **Project**](https://github.com/trycatchh/distrofuse)
- [Follow our **Organization**](https://github.com/trycatchh)