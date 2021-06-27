#!/bin/bash
#
# Copyright 2020 NXP
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this list
# of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


# File contents:
#
# 1. help string
# 2. enabled features list
# 3. argument check
# 4. arguments to feature list processing
# 5. build board
# 6. main
#   1. echo selected configuration
#   2. for each enabled board:
#       - set environment vars for board selected
#       - call build_board()

# 1. help string
usage() { echo "Usage: $0 [-b BOARD_1] [-b BOARD_2] [-t TARGET_1] .. [-c]"        1>&2
          echo ""                                                                 1>&2
          echo "Description:          "                                           1>&2
          echo "          [-b|-board]"                                            1>&2
          echo "            { all,"                                               1>&2
          echo "              8M, NXPEVK_iMX8M_4GB,"                              1>&2
          echo "              8Mm, NXPEVK_iMX8M_Mini_2GB"                         1>&2
          echo "              8Mn, EVK_iMX8MN_2GB }"                              1>&2
          echo ""                                                                 1>&2
          echo "          [-t|-target_app] "                                      1>&2
          echo "            { all, u|uboot, optee, apps|tee_apps,"                1>&2
          echo "              uimg|uboot_image, tools|uefi_tools, uefi }"         1>&2
          echo ""                                                                 1>&2
          echo "          [-c|-clean]"                                            1>&2
          echo "          [-h|-help]"                                             1>&2
          echo "          [-bc|build_configuration]"                              1>&2
          echo "            { release|RELEASE - for Release version of uefi,"     1>&2
          echo "              debug|DEBUG     - for Debug version of uefi }"      1>&2
          echo "------------------------------------------------------------"     1>&2 ;
          exit 1; }


# 2. enabled features list
# features are enabled later by script parameters

script_name=$0
build_8m=0
build_8m_mini=0
build_8m_nano=0

clean=0
build_uboot=0
build_atf=0
build_optee=0
build_tee_apps=0
build_uboot_image=0
build_uefi_tools=0
build_uefi=0

build_all_selected=0
build_configuration=RELEASE

# 3. argument check
# prints help if no arguments

if [ $# -eq 0 ]; then
    echo No arguments specified.
    usage
    exit 0
fi

# 4. arguments to feature list processing
# process arguments one by one in switch-case using shift
#
# variables:
#   - #$
#       - number of arguments left
#   - $1
#       - first argument:   name  (ie. -t    in '-t uboot')
#   - $2
#       - second argument:  value (eg. uboot in '-t uboot')

while test $# -gt 0
do
    case "$1" in

    -c|-clean)
        clean=1
        ;;
    -h|-help)
        usage
        ;;
    -t|-target_app)
        case "${2}" in
            u|uboot)
                build_uboot=1
                ;;
            atf)
                build_atf=1
                ;;
            tee|optee)
                build_optee=1
                ;;
            apps|tee_apps)
                build_optee=1
                build_tee_apps=1
                ;;
            uimg|uboot_image)
                build_atf=1
                build_uboot=1
                build_optee=1
                build_uboot_image=1
                ;;
            tools|uefi_tools)
                build_uefi_tools=1
                ;;
            efi|uefi)
                build_uefi=1
                ;;
            all)
                build_all_selected=1
                build_uboot=1
                build_atf=1
                build_optee=1
                build_tee_apps=1
                build_uboot_image=1
                build_uefi_tools=1
                build_uefi=1
                ;;
            *)
                echo "Unknown option ${2} specified for -b --build"
                usage
                ;;
        esac
        shift
        ;;
    -b|-board)
        case "${2}" in
            8M|NXPEVK_iMX8M_4GB)
                build_8m=1
                ;;
            8Mm|NXPEVK_iMX8M_Mini_2GB)
                build_8m_mini=1
                ;;
            8Mn|EVK_iMX8MN_2GB)
                build_8m_nano=1
                echo "Nano is not supported"
                ;;
            all)
                build_8m=1
                build_8m_mini=1
                build_8m_nano=1
                ;;
            *)
                echo "Unknown option ${2} specified for -b --build"
                usage
                ;;
        esac
        shift
        ;;
    -bc|build_configuration)
        case "${2}" in
            release|RELEASE)
                build_configuration=RELEASE
                ;;
            debug|DEBUG)
                build_configuration=DEBUG
                ;;
            *)
            echo "Unknown option ${2} specified for -v"
            usage
            ;;
        esac
        shift
        ;;
    *)
        echo "Unknown option ${1} specified"
        usage
        ;;
    esac
    shift
done


# 5. definition of build board function

build_board () {
    if [ -z "$AARCH64_TOOLCHAIN_PATH" ] ; then
        echo "Toolchain path missing."
        echo "Example: export AARCH64_TOOLCHAIN_PATH=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-"
        exit
    fi

    # Prepare env vars
    export CROSS_COMPILE=$AARCH64_TOOLCHAIN_PATH
    export CROSS_COMPILE64=$AARCH64_TOOLCHAIN_PATH
    export GCC5_AARCH64_PREFIX=$AARCH64_TOOLCHAIN_PATH
    export ARCH=arm64


    if [ $build_uboot -eq 1 ]; then
        pushd u-boots/u-boot-imx_arm64/ || exit $?
        if [ $clean -eq 1 ]; then
            make clean
        fi
        make $uboot_defconfig || exit $?
        make -s -j2 || exit $?
        popd
    fi

    if [ $build_atf -eq 1 ]; then
        pushd imx-atf || exit $?
        if [ $clean -eq 1 ]; then
            make clean || exit $?
        fi
        make -s PLAT=$atf_plat SPD=opteed bl31 || exit $?
        popd
    fi

    if [ $build_optee -eq 1 ]; then
        export -n CROSS_COMPILE
        export -n ARCH
        pushd optee_os/optee_os_arm64 || exit $?
        if [ $clean -eq 1 ]; then
            make clean PLATFORM=imx PLATFORM_FLAVOR=$optee_plat || exit $?
            rm -f ./out/arm-plat-imx/tee.bin
        fi
        make -s -j2 PLATFORM=imx PLATFORM_FLAVOR=$optee_plat CFG_TEE_CORE_DEBUG=n CFG_TEE_CORE_LOG_LEVEL=2 CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=131072 || exit $?
        # debug
        # make PLATFORM=imx PLATFORM_FLAVOR=mx8mqevk \
        #  CFG_TEE_CORE_DEBUG=y CFG_TEE_CORE_LOG_LEVEL=3 \
        #  CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n \
        #  CFG_TA_DEBUG=y CFG_TEE_CORE_TA_TRACE=1 CFG_TEE_TA_LOG_LEVEL=2 \
        #  CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=131072

        ${CROSS_COMPILE64}objcopy -O binary ./out/arm-plat-imx/core/tee.elf ./out/arm-plat-imx/tee.bin || exit $?
        popd
        export CROSS_COMPILE=$AARCH64_TOOLCHAIN_PATH
        export ARCH=arm64
    fi

    if [ $build_tee_apps -eq 1 ]; then
        export TA_CROSS_COMPILE=$AARCH64_TOOLCHAIN_PATH
        export TA_DEV_KIT_DIR=../../../../optee_os/optee_os_arm64/out/arm-plat-imx/export-ta_arm64
        export TA_CPU=cortex-a53

        pushd MSRSec/TAs/optee_ta && \
        make CFG_ARM64_ta_arm64=y CFG_FTPM_USE_WOLF=y CFG_AUTHVARS_USE_WOLF=y || exit $?
        popd

        cp MSRSec/TAs/optee_ta/out/AuthVars/2d57c0f7-bddf-48ea-832f-d84a1a219301.ta  mu_platform_nxp/Microsoft/OpteeClientPkg/Bin/AuthvarsTa/Arm64/Test/ && \
        cp MSRSec/TAs/optee_ta/out/AuthVars/2d57c0f7-bddf-48ea-832f-d84a1a219301.elf mu_platform_nxp/Microsoft/OpteeClientPkg/Bin/AuthvarsTa/Arm64/Test/ && \
        cp MSRSec/TAs/optee_ta/out/fTPM/bc50d971-d4c9-42c4-82cb-343fb7f37896.ta mu_platform_nxp/Microsoft/OpteeClientPkg/Bin/fTpmTa/Arm64/Test/     && \
        cp MSRSec/TAs/optee_ta/out/fTPM/bc50d971-d4c9-42c4-82cb-343fb7f37896.elf mu_platform_nxp/Microsoft/OpteeClientPkg/Bin/fTpmTa/Arm64/Test/     || exit $?

        export -n TA_CROSS_COMPILE
        export -n TA_DEV_KIT_DIR
        export -n TA_CPU
    fi

    if [ $build_uboot_image -eq 1 ]; then
        pushd imx-mkimage/iMX8M || exit $?
        cp -f ../../firmware-imx-8.1/firmware/ddr/synopsys/lpddr4_pmu_train_*.bin . && \
        cp -f ../../firmware-imx-8.1/firmware/ddr/synopsys/ddr4_*.bin .             && \
        cp -f ../../firmware-imx-8.1/firmware/hdmi/cadence/signed_hdmi_imx8m.bin .  && \
        cp -f ../../optee_os/optee_os_arm64/out/arm-plat-imx/tee.bin .              && \
        cp -f ../../imx-atf/build/"$atf_plat"/release/bl31.bin .                    && \
        cp -f ../../u-boots/u-boot-imx_arm64/u-boot-nodtb.bin  .                    && \
        cp -f ../../u-boots/u-boot-imx_arm64/spl/u-boot-spl.bin .                   && \
        cp -f ../../u-boots/u-boot-imx_arm64/arch/arm/dts/"$uboot_dtb" .            && \
        cp -f ../../u-boots/u-boot-imx_arm64/tools/mkimage .                        && \
        mv mkimage mkimage_uboot                                                    || exit $?

        cd ..
        if [ $atf_plat == "imx8mn" ]; then
            make SOC=$mkimage_SOC flash_ddr4_evk || exit $?
        else
            make SOC=$mkimage_SOC flash_hdmi_spl_uboot || exit $?
        fi
        popd
        cp imx-mkimage/iMX8M/flash.bin imx-iotcore/build/board/"$bsp_folder"/Package/BootLoader/flash.bin || exit $?
    fi

    if [ $build_uefi_tools -eq 1 ]; then
        pushd mu_platform_nxp && \
        cd MU_BASECORE        || exit $?
        if [ $clean -eq 1 ]; then
            cd BaseTools      && \
            make clean        || exit $?
            cd ..
        fi
        make -s -C BaseTools  || exit $?
        cd ..
        popd
    fi

    if [ $build_uefi -eq 1 ]; then
        pushd mu_platform_nxp || exit $?
        if [ $clean -eq 1 ]; then
            rm -rf Build
            rm -rf Config
            python3 NXP/"${uefi_folder}"/PlatformBuild.py --setup || exit $?
        fi
        python3 NXP/"${uefi_folder}"/PlatformBuild.py -V TARGET=$build_configuration \
        PROFILE=DEV MAX_CONCURRENT_THREAD_NUMBER=10 || exit $?

        pushd Build/"${uefi_folder}"/${build_configuration}_GCC5/FV || exit $?
        cp ../../../../../imx-iotcore/build/firmware/its/uefi_imx8_unsigned.its .                   && \
        ../../../../../u-boots/u-boot-imx_arm64/tools/mkimage -f uefi_imx8_unsigned.its -r uefi.fit || exit $?
        popd
        popd
        cp mu_platform_nxp/Build/"${uefi_folder}"/${build_configuration}_GCC5/FV/uefi.fit imx-iotcore/build/board/"$bsp_folder"/Package/BootFirmware/uefi.fit || exit $?
    fi
    echo "-----------------------------------------------------------------"
    if [ $build_uboot_image -eq 1 ]; then
        echo "flash.bin copied to:" imx-iotcore/build/board/"${bsp_folder}"/Package/BootLoader/flash.bin || exit $?
    fi
    if [ $build_uefi -eq 1 ]; then
        echo "uefi.fit copied to:" imx-iotcore/build/board/"${bsp_folder}"/Package/BootFirmware/uefi.fit || exit $?
    fi

    echo "Done"
}

# 6. main
# the main function of this script
#
#   6.1. echo selected configuration

echo building bootloader image for Windows 10 IOT core
echo "Build Configuration: " $build_configuration
echo -----------------
echo
echo Selected configuration:
echo "clean              $clean"
echo "build_uboot        $build_uboot"
echo "build_atf          $build_atf"
echo "build_optee        $build_optee"
echo "build_tee_apps     $build_tee_apps"
echo "build_uboot_image  $build_uboot_image"
echo "build_uefi_tools   $build_uefi_tools"
echo "build_uefi         $build_uefi"
echo -----------------
if [ $build_all_selected -eq 0 ]; then
    echo
    echo Warning \"-t all\" not set! Build of reduced feature set should be used for development purposes only and requires \"$script_name -t all\" to be run first.
fi
echo
echo Building..
echo

#  6.2. for each board
#       - set environment vars for board selected
#       - call build_board()
#
#  > Note: '$?' contains return value

# available configs bellow:
#   1) i.MX 8MQ EVK config
#   2) i.MX 8M Mini EVK config
#   3) i.MX 8M Nano EVK config

# i.MX 8MQ EVK config
if [ $build_8m -eq 1 ]; then
    echo "Board type IMX8M EVK"
    bsp_folder="NXPEVK_iMX8M_4GB"
    uefi_folder="MCIMX8M_EVK_4GB"
    uboot_defconfig="imx8mq_evk_nt_defconfig"
    atf_plat="imx8mq"
    optee_plat="mx8mqevk"
    uboot_dtb="fsl-imx8mq-evk.dtb"
    mkimage_SOC="iMX8M"

    build_board
    # check return value
    if [ $? -gt 0 ]; then
        exit $?
    fi
fi

# i.MX 8M Mini EVK config
if [ $build_8m_mini -eq 1 ]; then
    echo "Board type IMX8MM EVK"
    bsp_folder="NXPEVK_iMX8M_Mini_2GB"
    uefi_folder="MCIMX8M_MINI_EVK_2GB"
    uboot_defconfig="imx8mm_evk_nt_defconfig"
    atf_plat="imx8mm"
    optee_plat="mx8mmevk"
    uboot_dtb="fsl-imx8mm-evk.dtb"
    mkimage_SOC="iMX8MM"

    build_board
    # check return value
    if [ $? -gt 0 ]; then
        exit $?
    fi
fi

# i.MX 8M Nano EVK config
if [ $build_8m_nano -eq 1 ]; then
echo "Board type IMX8MN EVK"
    bsp_folder="EVK_iMX8MN_2GB"
    uefi_folder=${bsp_folder}
    uboot_defconfig="imx8mn_ddr4_evk_nt_defconfig"
    atf_plat="imx8mn"
    optee_plat="mx8mnevk"
    uboot_dtb="fsl-imx8mn-ddr4-evk.dtb"
    mkimage_SOC="iMX8MN"

    build_board
    # check return value
    echo Build has returned $? !
    if [ $? -gt 0 ]; then
        exit $?
    fi
fi
