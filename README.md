# Introduction
The tool can be used to eliminate redundant instructions in a basic block, thus making the reverse analysis work simple and easy to carry out. The core ideas used are stain analysis and active variable analysis!  
# Install 
## Environment
- Language: C++
- IDE: VS2022
- External Dependencies: Capstone & Boost
## Code
You need clone this repo, the source code in the "clean_basic_block\src", the lib files in the "clean_basic_block\lib", the DLL files in the "clean_basic_block\dll", the header file in the "clean_basic_block\include".  
## Realse
You also can download the realse file to use this project.
# Usage
![README-2023-05-21-19-12-28](https://g0mx-picbed.oss-cn-beijing.aliyuncs.com/blogs/pictures/README-2023-05-21-19-12-28.png)
This tools have four optinos: 
- "-h" 为显示帮助信息  
- "-f" 为包含待分析基本块的机器码文件，该选项为必选项  
- "-s" 为第一条指令的起始地址(RVA)，该选项为必选项
- "-e" 为基本块的结束地址，该选项为必选项
# Example
This basic block has been obfuscated:  
```x86asm
.vmp1:0064D713 56                            push    esi
.vmp1:0064D714 0F BF F1                      movsx   esi, cx
.vmp1:0064D717 BE A3 3B 66 6B                mov     esi, 6B663BA3h
.vmp1:0064D71C 55                            push    ebp
.vmp1:0064D71D 52                            push    edx
.vmp1:0064D71E 51                            push    ecx
.vmp1:0064D71F 9C                            pushf
.vmp1:0064D720 F5                            cmc
.vmp1:0064D721 0F BF EF                      movsx   ebp, di
.vmp1:0064D724 87 D6                         xchg    edx, esi
.vmp1:0064D726 50                            push    eax
.vmp1:0064D727 98                            cwde
.vmp1:0064D728 57                            push    edi
.vmp1:0064D729 80 E6 1A                      and     dh, 1Ah
.vmp1:0064D72C 87 FF                         xchg    edi, edi
.vmp1:0064D72E 53                            push    ebx
.vmp1:0064D72F 66 F7 C6 9C 51                test    si, 519Ch
.vmp1:0064D734 66 0F A4 E7 8F                shld    di, sp, 8Fh
.vmp1:0064D739 BA 00 00 00 00                mov     edx, 0
.vmp1:0064D73E 85 EE                         test    esi, ebp
.vmp1:0064D740 66 2B C7                      sub     ax, di
.vmp1:0064D743 D2 CF                         ror     bh, cl
.vmp1:0064D745 52                            push    edx
.vmp1:0064D746 66 98                         cbw
.vmp1:0064D748 8B 6C 24 28                   mov     ebp, [esp+24h+arg_0]
.vmp1:0064D74C 81 ED 25 26 6F 1D             sub     ebp, 1D6F2625h
.vmp1:0064D752 0F CD                         bswap   ebp
.vmp1:0064D754 0F 47 C5                      cmova   eax, ebp
.vmp1:0064D757 98                            cwde
.vmp1:0064D758 F6 C4 F5                      test    ah, 0F5h
.vmp1:0064D75B F7 DD                         neg     ebp
.vmp1:0064D75D 66 0F BD F2                   bsr     si, dx
.vmp1:0064D761 66 0F BA F7 76                btr     di, 76h ; 'v'
.vmp1:0064D766 66 F7 C7 A9 43                test    di, 43A9h
.vmp1:0064D76B F7 D5                         not     ebp
.vmp1:0064D76D 66 0F AC C0 D8                shrd    ax, ax, 0D8h
.vmp1:0064D772 81 ED 7B 53 BE 38             sub     ebp, 38BE537Bh
.vmp1:0064D778 66 0F BA FF 07                btc     di, 7
.vmp1:0064D77D 66 98                         cbw
.vmp1:0064D77F C1 C5 03                      rol     ebp, 3
.vmp1:0064D782 66 0F A4 F0 AB                shld    ax, si, 0ABh
.vmp1:0064D787 2B DE                         sub     ebx, esi
.vmp1:0064D789 8D 6C 15 00                   lea     ebp, [ebp+edx+0]
.vmp1:0064D78D 8B F4                         mov     esi, esp
.vmp1:0064D78F 66 0F BA F2 E5                btr     dx, 0E5h
.vmp1:0064D794 66 0B D8                      or      bx, ax
.vmp1:0064D797 47                            inc     edi
.vmp1:0064D798 81 EC C0 00 00 00             sub     esp, 0C0h
.vmp1:0064D79E 66 3B F8                      cmp     di, ax
.vmp1:0064D7A1 F9                            stc
.vmp1:0064D7A2 66 85 EB                      test    bx, bp
```  
The project is used for cleaning the above basic block:  
```ps1
(base) PS C:\Users\g0mx> D:\Code_Files\Deobfusecate\Release\Clean_BB.exe -f D:\Code_Files\EliminateVmpJunkCode\Project1.vmp\Project1.vmp.exe -s "0x64D713" -e "0x64D7A5"
```
After cleaning:  
```x86asm
Insn List After Clean:
0x64d713:       push            esi
0x64d71c:       push            ebp
0x64d71d:       push            edx
0x64d71e:       push            ecx
0x64d71f:       pushfd
0x64d726:       push            eax
0x64d728:       push            edi
0x64d72c:       xchg            edi, edi
0x64d72e:       push            ebx
0x64d734:       shld            di, sp, 0x8f
0x64d761:       btr             di, 0x76
0x64d778:       btc             di, 7
0x64d797:       inc             edi
clean over!!!
```  