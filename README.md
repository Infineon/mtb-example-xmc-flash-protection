# XMC MCU: Flash Protection

This example demonstrates how flash protection can be configured in the XMC™ 4000 Series of devices.

**Note:** This code example involves programming the User Configuration Blocks (UCB), which can only be programmed only a limited number of times. See the [Design and Implementation](#design-and-implementation) section before proceeding with this code example.

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.3
- [SEGGER J-Link software](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- Programming Language: C
- Associated Parts: [XMC™ 4000 Series MCU](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/) parts

## Supported Toolchains (make variable 'TOOLCHAIN')

- GNU Arm® Embedded Compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`

## Supported Kits (make variable 'TARGET')

- [XMC4700 Relax Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_v1/) (`KIT_XMC47_RELAX_V1`) - Default value of `TARGET`

## Hardware Setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.

## Software Setup

- Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).

## Using the Code Example

Create the project and open it using one of the following:

<details open><summary><b>In Eclipse IDE for ModusToolbox</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](http://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. Optionally, change the suggested **New Application Name**.

5. Enter the local path in the **Application(s) Root Path** field to indicate where the application needs to be created.

   Applications that can share libraries can be placed in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details open><summary><b>In Command-line Interface (CLI)</b></summary>

ModusToolbox provides the Project Creator as both a GUI tool and a command line tool to easily create one or more ModusToolbox applications. See the "Project Creator Tools" section of the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) for more details.

Alternatively, you can manually create the application using the following steps:

1. Download and unzip this repository onto your local machine, or clone the repository.

2. Open a CLI terminal and navigate to the application folder.

   On Linux and macOS, you can use any terminal application. On Windows, open the **modus-shell** app from the Start menu.

   **Note:** The cloned application contains a default BSP file (*TARGET_xxx.mtb*) in the *deps* folder. Use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) (`make modlibs` command) to select and download a different BSP file, if required. If the selected kit does not have the required resources or is not [supported](#supported-kits-make-variable-target), the application may not work.

3. Import the required libraries by executing the `make getlibs` command.

Various CLI tools include a `-h` option that prints help information to the terminal screen about that tool. For more details, see the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details open><summary><b>In Third-party IDEs</b></summary>

**Note:** Only VS Code is supported.

1. Follow the instructions from the **In Command-line Interface (CLI)** section to create the application, and import the libraries using the `make getlibs` command.

2. Export the application to a supported IDE using the `make <ide>` command.

   For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*).

3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

</details>

## Operation

1. Connect the board to your PC using a micro-USB cable through the debug USB connector.

2. Program the board using Eclipse IDE for ModusToolbox:

   1. Select the application project in the Project Explorer.

   2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (JLink)**.

3. Once programmed, open your serial terminal software. You should see the shell application displayed as shown below:

   **Figure 1. Shell Application**

   ![](images/shell_start.png)

4. Do the following to install flash read protection using the shell application:

   1. Type `status` to get the status of flash protection and UCB program count:

      ```
      >> status
      ```

      **Figure 2. Protection Status**

      ![](images/shell_status1.png)

   2. Type `install <password0> <password1>` to install flash protection.

      The device will reset after this command for the changes to take effect.

      For example:

      ```
      >> install 1234 5678
      ```

      **Figure 3. Install Protection**

      ![](images/shell_install.png)

   3. Check the status to see if the flash protection was successful. The UCB program count will also be displayed.

      ```
      >> status
      ```

      **Figure 4. Protection Status**

      ![](images/shell_status2.png)

   4. Type `disable <password0> <password1>` to disable the flash protection temporarily. Use the same credentials used in Step 2.

      ```
      >> disable 1234 5678
      ```

      **Figure 5. Disable Protection**

      ![](images/shell_disable.png)

   5. Type `resume` to re-enable flash protection installed previously:

      ```
      >> resume
      ```

      **Figure 6. Resume Protection**

      ![](images/shell_resume.png)

   6. Type `uninstall <password0> <password1>` to uninstall flash protection. Use the same credentials used in Step 2:

      The device will reset after this command for the changes to take effect.

      ```
      >> uninstall 1234 5678
      ```

      **Figure 7. Uninstall Protection**

      ![](images/shell_uninstall.png)

   7. Check the status to see if flash protection was uninstalled successfully:

      ```
      >> status
      ```

      **Figure 8. Protection Status**

      ![](images/shell_status3.png)

<br>

## Design and Implementation

**User Configuration Blocks (UCB)** are used to configure the flash read/write protection on XMC4000 series of devices. There are three UCBs that can be programmed to achieve this: UCB0, UCB1, and UCB2.

In this code example, UCB0 is programmed to implement global flash read protection. The code example firmware flow is illustrated below:

**Figure 9. Firmware Flow Diagram**

![](images/block_diagram.png)

The code example uses a shell application to take input commands from the user to perform five different operations:

Command | Arguments | Description
------- | --------- | -----------------
`install` | `<pwd0> <pwd1>` |  Programs global flash read protection on the flash rows with passwords `pwd0` and `pwd1`
`uninstall` | `<pwd0> <pwd1>` | Removes flash protection with previously configured passwords
`disable` | `<pwd0> <pwd1>` | Disables flash protection temporarily with previously configured passwords
`resume` | None | Re-enables previously configured flash protection
`status` | None | Displays the flash protection status

<br>

**Note:** The protection installation is modified by erasing and programming the UCBs with dedicated command sequences as described in the firmware flow diagram. **The erase and re-program of the UCBs can be performed up to four times. Exceeding this limit may make the device unusable.**

## Resources and Settings

The code example uses a custom *design.modus* file because it uses the **Universal Serial Interface Channel (USIC)** block to implement the shell application. The following settings were modified in the default *design.modus* file.

**Figure 10. USIC (UART) Settings**

![](images/design_modus.png)


**Figure 11. UART Rx Settings**

![](images/rx.png)


**Figure 12. UART Tx Settings**

![](images/tx.png)

## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (JLink)** configuration in the **Quick Panel**. For more details, see the "Program and Debug" section in the [Eclipse IDE for ModusToolbox User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide).

## Related Resources

| Kit Guides                                            |                                                             |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [XMC4700/XMC4800 Relax Kit Series-V1](https://www.infineon.com/dgdl/Infineon-Board_User_Manual_XMC4700_XMC4800_Relax_Kit_Series-UM-v01_02-EN.pdf?fileId=5546d46250cc1fdf01513f8e052d07fc) – Board User‘s Manual | Describes the schematic and hardware of the XMC4700/XMC4800 Relax Kit Series-V1, equipped with an Arm® Cortex®-M4 based XMC™ Microcontroller from Infineon Technologies AG. |
| **Code Examples**                                            |
| [Using ModusToolbox](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software) |
| **Device Documentation**                                     |
| [XMC4000 Family Datasheets](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/#document-group-myInfineon-49) | [XMC4000 Family Technical Reference Manuals](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/#document-group-myInfineon-44) |
| **Development Kits**                                         | Buy at www.infineon.com                                       |
| [KIT_XMC47_RELAX_V1](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_v1/) XMC4700 Relax Kit |
| **Libraries**                                                 |                                                              |
| XMC Peripheral Library (XMCLib) and docs  | [mtb-xmclib-cat3](https://github.com/cypresssemiconductorco/mtb-xmclib-cat3) on GitHub |
| **Tools**                                                    |                                                              |
| [Eclipse IDE for ModusToolbox](https://www.cypress.com/modustoolbox)     | The cross-platform, Eclipse-based IDE for IoT designers that supports application configuration and development targeting converged MCU and wireless systems.             |

## Other Resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

For XMC MCU devices, see [32-bit XMC™ Industrial Microcontroller based on Arm® Cortex®-M](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/).

## Document History

Document Title: *CE232719* - *XMC MCU: Flash Protection*

| Version | Description of Change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
------

All other trademarks or registered trademarks referenced herein are the property of their respective owners.

![banner](images/ifx_logo_rgb.jpg)

© 2021 Infineon Technologies AG

All Rights Reserved.

### Legal Disclaimer

The information given in this document shall in no event be regarded as a guarantee of conditions or characteristics. With respect to any examples or hints given herein, any typical values stated herein and/or any information regarding the application of the device, Infineon Technologies hereby disclaims any and all warranties and liabilities of any kind, including without limitation, warranties of non-infringement of intellectual property rights of any third party.

### Information

For further information on technology, delivery terms and conditions and prices, please contact the nearest Infineon Technologies Office (www.infineon.com).

### Warnings

Due to technical requirements, components may contain dangerous substances. For information on the types in question, please contact the nearest Infineon Technologies Office.

Infineon Technologies components may be used in life-support devices or systems only with the express written approval of Infineon Technologies, if a failure of such components can reasonably be expected to cause the failure of that life-support device or system or to affect the safety or effectiveness of that device or system. Life support devices or systems are intended to be implanted in the human body or to support and/or maintain and sustain and/or protect human life. If they fail, it is reasonable to assume that the health of the user or other persons may be endangered.

-------------------------------------------------------------------------------
