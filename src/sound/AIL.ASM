; /* target: seg022_2405 */
; /* opts: /ml */
;
; The Miles Audio Interface Library (AIL 2.0) application interface: the timer services
; (API_timer replaces the BIOS timer interrupt and runs up to 16 registered callbacks at
; their own rates, with the BIOS handler chained as the 17th), driver registration, and
; the AIL_ calls that only pass a function number to the driver (mov ax,N; jmp call_driver).
;
; Hand-written assembly: its data lives in the code segment (state from +0, a 512-byte
; private stack the timer interrupt switches to, the 'Test' marker the handler checks),
; and the 8086 `push imm` sequences (push ax; push bp; mov bp,sp; mov [bp+2],imm; pop bp)
; show it was assembled without .186, which TASM 2.0 expands that way. POP_F is the
; usual guard against the early 80286 popf bug.
;
; Names: the FM Towns build carries the same library, so every routine has its FM name.
; The AIL_ dispatch stubs pass the same function numbers in both builds, which names the
; ones no C caller reaches; the rest were matched by what they do (find_proc walks the
; driver's function table, API_timer is the interrupt handler, init_DDA_arrays clears the
; timer tables, set_PIT_divisor programs the PIT, and so on). seg022_2405_5F3 is an
; unsigned 32-bit division with no FM counterpart (FM divides natively).
; The data labels are left as L + offset.

        .model  medium

; popf without the early 80286 bug: iret pops the flags pushf saved
POP_F   macro
        local   iret_op
        db      80h                     ; or bh,0 ...
iret_op:
        db      0CFh,0                  ; ... hiding an iret
        push    cs
        call    near ptr iret_op
        endm


        .code

        public  _find_proc
        public  _call_driver
        public  _API_timer
        public  _init_DDA_arrays
        public  _bios_caller
        public  _hook_timer_process
        public  _unhook_timer_process
        public  _set_PIT_divisor
        public  _set_PIT_period
        public  _seg022_2405_5F3
        public  _program_timers
        public  _AIL_startup
        public  _AIL_shutdown
        public  _AIL_register_timer
        public  _AIL_release_timer_handle
        public  _AIL_release_all_timers
        public  _AIL_start_timer
        public  _AIL_start_all_timers
        public  _AIL_stop_timer
        public  _AIL_stop_all_timers
        public  _AIL_set_timer_period
        public  _AIL_set_timer_frequency
        public  _AIL_set_timer_divisor
        public  _AIL_interrupt_divisor
        public  _AIL_register_driver
        public  _AIL_release_driver_handle
        public  _AIL_describe_driver
        public  _AIL_detect_device
        public  _AIL_init_driver
        public  _AIL_shutdown_driver
        public  _AIL_index_VOC_block
        public  _AIL_register_sound_buffer
        public  _AIL_format_sound_buffer
        public  _AIL_sound_buffer_status
        public  _AIL_play_VOC_file
        public  _AIL_format_VOC_file
        public  _AIL_VOC_playback_status
        public  _AIL_start_digital_playback
        public  _AIL_stop_digital_playback
        public  _AIL_pause_digital_playback
        public  _AIL_resume_digital_playback
        public  _AIL_set_digital_playback_volume
        public  _AIL_digital_playback_volume
        public  _AIL_set_digital_playback_panpot
        public  _AIL_digital_playback_panpot
        public  _AIL_state_table_size
        public  _AIL_register_sequence
        public  _AIL_release_sequence_handle
        public  _AIL_default_timbre_cache_size
        public  _AIL_define_timbre_cache
        public  _AIL_timbre_request
        public  _AIL_install_timbre
        public  _AIL_protect_timbre
        public  _AIL_unprotect_timbre
        public  _AIL_timbre_status
        public  _AIL_start_sequence
        public  _AIL_stop_sequence
        public  _AIL_resume_sequence
        public  _AIL_sequence_status
        public  _AIL_relative_volume
        public  _AIL_relative_tempo
        public  _AIL_set_relative_volume
        public  _AIL_set_relative_tempo
        public  _AIL_beat_count
        public  _AIL_measure_count
        public  _AIL_branch_index
        public  _AIL_controller_value
        public  _AIL_set_controller_value
        public  _AIL_channel_notes
        public  _AIL_send_channel_voice_message
        public  _AIL_send_sysex_message
        public  _AIL_write_display
        public  _AIL_install_callback
        public  _AIL_cancel_callback
        public  _AIL_lock_channel
        public  _AIL_map_sequence_channel
        public  _AIL_release_channel
        public  _AIL_true_sequence_channel

L0000:
        db      0h,0h
L0002:
        db      0h,0h
L0004:
        db      0h,0h
L0006:
        db      3Eh dup (0)
L0044:
        db      0h,0h
L0046:
        db      0h,0h
L0048:
        db      22h dup (0)
L006A:
        db      20h dup (0)
L008A:
        db      0h,0h
L008C:
        db      0h,0h
L008E:
        db      42h dup (0)
L00D0:
        db      0h,0h
L00D2:
        db      42h dup (0)
L0114:
        db      0h,0h
L0116:
        db      0h,0h
L0118:
        db      0h,0h
L011A:
        db      0h,0h
L011C:
        db      0h,0h
L011E:
        db      0h,0h
L0120:
        db      0h,0h
L0122:
        db      0h,0h
L0124:
        db      0h,0h
L0126:
        db      3Eh dup (0)
L0164:
        db      20h dup (0)
L0184:
        db      24h dup (0)
L01A8:
        db      0h,0h,0h,0h,0h,0h
L01AE:
        db      0h,0h
L01B0:
        db      54h,65h
L01B2:
        db      73h,74h
        db      200h dup (0)
L03B4:
        db      0h,0h
L03B6:
        db      0h,0h
L03B8:
        db      0D3h,0h
; FindEntryInAdvFile_seg022_2405_3BA  (+3BA)
_find_proc proc far
        cmp     bx,10h
        jae     short L03E4
        shl     bx,1
        shl     bx,1
        les     bx,dword ptr cs:L0124[bx]
        mov     cx,es
        or      cx,bx
        je      short L03E4
L03CE:
        mov     cx,word ptr es:[bx]
        cmp     cx,ax
        je      short L03EB
        add     bx,4
        cmp     cx,0FFFFh
        jne     L03CE
        mov     ax,0
        mov     dx,0
        retf
L03E4:
        mov     ax,0
        mov     dx,0
        retf
L03EB:
        mov     ax,word ptr es:[bx+2]
        mov     dx,es
        retf
_find_proc endp

; PushAdvTableEntry_seg022_2405_3F2  (+3F2)
_call_driver proc far
        mov     bx,sp
        mov     bx,word ptr ss:[bx+4]
        call    _find_proc
        cmp     ax,0
        jne     short L0406
        cmp     dx,0
        je      short L0409
L0406:
        push    dx
        push    ax
        retf
L0409:
        retf
_call_driver endp

; TimerInterupt_seg022_2405_40A  (+40A)
_API_timer proc far
        cmp     word ptr cs:L0002,0
        je      short L0415
        jmp     L04C4
L0415:
        mov     word ptr cs:L0002,1
        cld
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    bp
        push    es
        push    ds
        mov     word ptr cs:L03B4,ss
        mov     word ptr cs:L03B6,sp
        mov     ax,cs
        mov     ss,ax
        mov     sp,offset L03B4                 ; the private stack below the saved SS:SP
        mov     word ptr cs:L011C,0
L043E:
        mov     si,word ptr cs:L011C
        shl     si,1
        cmp     word ptr cs:L006A[si],2
        jne     short L049D
        mov     ds,word ptr cs:L0048[si]
        shl     si,1
        mov     ax,word ptr cs:L008C[si]
        mov     dx,word ptr cs:L008E[si]
        add     ax,word ptr cs:L0114
        adc     dx,word ptr cs:L0116
        cmp     dx,word ptr cs:L00D2[si]
        jb      short L0478
        ja      short L0484
        cmp     ax,word ptr cs:L00D0[si]
        jae     short L0484
L0478:
        mov     word ptr cs:L008C[si],ax
        mov     word ptr cs:L008E[si],dx
        jmp     short L049D
L0484:
        sub     ax,word ptr cs:L00D0[si]
        sbb     dx,word ptr cs:L00D2[si]
        mov     word ptr cs:L008C[si],ax
        mov     word ptr cs:L008E[si],dx
        call    dword ptr cs:L0004[si]
L049D:
        inc     word ptr cs:L011C
        cmp     word ptr cs:L011C,10h
        jbe     L043E
        mov     ss,word ptr cs:L03B4
        mov     sp,word ptr cs:L03B6
        pop     ds
        pop     es
        pop     bp
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        mov     word ptr cs:L0002,0
L04C4:
        push    ax
        mov     al,20h
        out     20h,al
        pop     ax
        cmp     word ptr cs:L01B0,6554h
        jne     short L04DD
        cmp     word ptr cs:L01B2,7473h
        jne     short L04DD
        iret
L04DD:
        sti
        int     3
        jmp     L04DD
_API_timer endp

; seg022_2405_4E1  (+4E1)
_init_DDA_arrays proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        cld
        mov     word ptr cs:L0114,0FFFFh
        mov     word ptr cs:L0116,0FFFFh
        push    cs
        pop     es
        mov     di,offset L006A
        mov     cx,11h
        mov     ax,0
        rep stosw
        mov     di,offset L008C
        mov     cx,22h
        rep stosw
        mov     di,offset L00D0
        mov     cx,22h
        rep stosw
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_init_DDA_arrays endp

; seg022_2405_51D  (+51D)
_bios_caller proc far
        pushf
        call    dword ptr cs:L0118
        retf
_bios_caller endp

; seg022_2405_524  (+524)
_hook_timer_process proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     ax,0
        mov     es,ax
        mov     bx,word ptr es:[20h]
        mov     es,word ptr es:[22h]
        mov     word ptr cs:L0118,bx
        mov     word ptr cs:L011A,es
        mov     bx,offset _bios_caller
        mov     word ptr cs:L0044,bx
        mov     word ptr cs:L0046,cs
        mov     ax,cs
        mov     ds,ax
        mov     dx,offset _API_timer
        mov     ax,0
        mov     es,ax
        mov     word ptr es:[20h],dx
        mov     word ptr es:[22h],ds
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_hook_timer_process endp

; seg022_2405_570  (+570)
_unhook_timer_process proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     word ptr cs:L011C,0FFFFh
        mov     dx,word ptr cs:L0118
        mov     ds,word ptr cs:L011A
        mov     ax,0
        mov     es,ax
        mov     word ptr es:[20h],dx
        mov     word ptr es:[22h],ds
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_unhook_timer_process endp

; seg022_2405_5A0  (+5A0)
_set_PIT_divisor proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     al,36h
        out     43h,al
        mov     ax,word ptr [bp+6]
        mov     word ptr cs:L0122,ax
        jmp     short L05B5
L05B5:
        out     40h,al
        mov     al,ah
        jmp     short L05BB
L05BB:
        out     40h,al
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_set_PIT_divisor endp

; seg022_2405_5C9  (+5C9)
_set_PIT_period proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        mov     ax,0
        cmp     word ptr [bp+6],0D68Dh
        jae     short L05E6
        mov     ax,word ptr [bp+6]
        mov     bx,20BCh
        mov     cx,2710h
        mul     cx
        div     bx
L05E6:
        push    ax
        call    _set_PIT_divisor
        add     sp,2
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_set_PIT_period endp

; seg022_2405_5F3  (+5F3)
_seg022_2405_5F3 proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        mov     ax,word ptr [bp+6]
        mov     dx,word ptr [bp+8]
        mov     bx,word ptr [bp+0Ah]
        mov     cx,word ptr [bp+0Ch]
        or      cx,cx
        jne     short L0611
        or      dx,dx
        je      short L0635
        or      bx,bx
        je      short L0635
L0611:
        mov     bp,cx
        mov     cx,20h
        xor     di,di
        xor     si,si
L061A:
        shl     ax,1
        rcl     dx,1
        rcl     si,1
        rcl     di,1
        cmp     di,bp
        jb      short L0631
        ja      short L062C
        cmp     si,bx
        jb      short L0631
L062C:
        sub     si,bx
        sbb     di,bp
        inc     ax
L0631:
        loop    L061A
        jmp     short L0639
L0635:
        div     bx
        xor     dx,dx
L0639:
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_seg022_2405_5F3 endp

; seg022_2405_63E  (+63E)
_program_timers proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        cld
        mov     word ptr cs:L011E,0FFFFh
        mov     word ptr cs:L0120,0FFFFh
        mov     si,0
L0655:
        mov     bx,si
        shl     bx,1
        cmp     word ptr cs:L006A[bx],0
        je      short L0686
        shl     bx,1
        mov     ax,word ptr cs:L00D0[bx]
        mov     dx,word ptr cs:L00D2[bx]
        cmp     dx,word ptr cs:L0120
        jb      short L067D
        ja      short L0686
        cmp     ax,word ptr cs:L011E
        jae     short L0686
L067D:
        mov     word ptr cs:L011E,ax
        mov     word ptr cs:L0120,dx
L0686:
        inc     si
        cmp     si,10h
        jbe     L0655
        mov     ax,word ptr cs:L011E
        mov     dx,word ptr cs:L0120
        cmp     ax,word ptr cs:L0114
        jne     short L06A3
        cmp     dx,word ptr cs:L0116
        je      short L06C8
L06A3:
        mov     word ptr cs:L011C,0FFFFh
        mov     word ptr cs:L0114,ax
        mov     word ptr cs:L0116,dx
        push    ax
        call    _set_PIT_period
        add     sp,2
        push    cs
        pop     es
        mov     di,offset L008C
        mov     cx,22h
        mov     ax,0
        rep stosw
L06C8:
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_program_timers endp

; seg022_2405_6D3  (+6D3)
_AIL_startup proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     word ptr cs:L0000,0
        mov     word ptr cs:L0002,0
        cld
        mov     ax,cs
        mov     es,ax
        mov     di,offset L0124
        mov     cx,20h
        mov     ax,0
        rep stosw
        mov     di,offset L0164
        mov     cx,10h
        mov     ax,0FFFFh
        rep stosw
        mov     di,offset L0184
        mov     cx,10h
        mov     ax,0
        rep stosw
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_AIL_startup endp

; seg022_2405_717  (+717)
_AIL_shutdown proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     word ptr cs:L01A8,0
L0726:
        mov     si,word ptr cs:L01A8
        shl     si,1
        mov     dx,word ptr cs:L0164[si]
        shl     si,1
        mov     ax,word ptr cs:L0124[si]
        or      ax,word ptr cs:L0126[si]
        je      short L075F
        cmp     dx,0FFFFh
        je      short L074D
        push    dx
        push    cs
        call    near ptr _AIL_release_timer_handle
        add     sp,2
L074D:
        push    word ptr [bp+8]
        push    word ptr [bp+6]
        push    word ptr cs:L01A8
        push    cs
        call    near ptr _AIL_shutdown_driver
        add     sp,6
L075F:
        inc     word ptr cs:L01A8
        cmp     word ptr cs:L01A8,10h
        jne     L0726
        push    cs
        call    near ptr _AIL_release_all_timers
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_shutdown endp

; seg022_2405_77C  (+77C)
_AIL_register_timer proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     cx,ds
        mov     bx,0
L0789:
        cmp     word ptr cs:L006A[bx],0
        je      short L079F
        add     bx,2
        cmp     bx,20h
        jb      L0789
        mov     ax,0FFFFh
        jmp     L082B
L079F:
        mov     ax,bx
        shr     ax,1
        mov     word ptr cs:L006A[bx],1
        mov     word ptr cs:L0048[bx],cx
        shl     bx,1
        lds     si,dword ptr [bp+6]
        mov     word ptr cs:L0004[bx],si
        mov     word ptr cs:L0006[bx],ds
        mov     word ptr cs:L00D0[bx],0FFFFh
        mov     word ptr cs:L00D2[bx],0FFFFh
        inc     word ptr cs:L0000
        cmp     word ptr cs:L0000,1
        jne     short L082B
        push    ax
        call    _init_DDA_arrays
        mov     word ptr cs:L008A,1
        call    _hook_timer_process
        push    0
        push    0D68Dh
        push    10h
        push    cs
        call    near ptr _AIL_set_timer_period
        add     sp,6
        push    10h
        push    cs
        call    near ptr _AIL_start_timer
        add     sp,2
        pop     ax
        mov     bx,ax
        shl     bx,1
        mov     word ptr cs:L006A[bx],1
L082B:
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_register_timer endp

; seg022_2405_837  (+837)
_AIL_release_timer_handle proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     bx,word ptr [bp+6]
        cmp     bx,0FFFFh
        je      short L0874
        shl     bx,1
        cmp     word ptr cs:L006A[bx],0
        je      short L0874
        mov     word ptr cs:L006A[bx],0
        dec     word ptr cs:L0000
        jne     short L0874
        push    0
        call    _set_PIT_divisor
        add     sp,2
        call    _unhook_timer_process
L0874:
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_release_timer_handle endp

; seg022_2405_880  (+880)
_AIL_release_all_timers proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     si,0Fh
L0888:
        push    si
        call    _AIL_release_timer_handle
        add     sp,2
        dec     si
        jge     L0888
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_AIL_release_all_timers endp

; seg022_2405_89E  (+89E)
_AIL_start_timer proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     bx,word ptr [bp+6]
        shl     bx,1
        cmp     word ptr cs:L006A[bx],1
        jne     short L08BA
        mov     word ptr cs:L006A[bx],2
L08BA:
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_start_timer endp

; seg022_2405_8C6  (+8C6)
_AIL_start_all_timers proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     si,0Fh
L08CE:
        push    si
        call    _AIL_start_timer
        add     sp,2
        dec     si
        jge     L08CE
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_AIL_start_all_timers endp

; seg022_2405_8E4  (+8E4)
_AIL_stop_timer proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     bx,word ptr [bp+6]
        shl     bx,1
        cmp     word ptr cs:L006A[bx],2
        jne     short L0900
        mov     word ptr cs:L006A[bx],1
L0900:
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_stop_timer endp

; seg022_2405_90C  (+90C)
_AIL_stop_all_timers proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     si,0Fh
L0914:
        push    si
        call    _AIL_stop_timer
        add     sp,2
        dec     si
        jge     L0914
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_AIL_stop_all_timers endp

; seg022_2405_92A  (+92A)
_AIL_set_timer_period proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     bx,word ptr [bp+6]
        shl     bx,1
        mov     ax,word ptr cs:L006A[bx]
        push    ax
        mov     word ptr cs:L006A[bx],1
        shl     bx,1
        mov     ax,word ptr [bp+8]
        mov     dx,word ptr [bp+0Ah]
        mov     word ptr cs:L00D0[bx],ax
        mov     word ptr cs:L00D2[bx],dx
        mov     word ptr cs:L008C[bx],0
        mov     word ptr cs:L008E[bx],0
        call    _program_timers
        pop     ax
        mov     bx,word ptr [bp+6]
        shl     bx,1
        mov     word ptr cs:L006A[bx],ax
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_set_timer_period endp

; seg022_2405_97F  (+97F)
_AIL_set_timer_frequency proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        push    word ptr [bp+0Ah]
        push    word ptr [bp+8]
        push    0Fh
        push    4240h
        call    _seg022_2405_5F3
        add     sp,8
        push    dx
        push    ax
        push    word ptr [bp+6]
        call    _AIL_set_timer_period
        add     sp,6
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
_AIL_set_timer_frequency endp

; seg022_2405_9BF  (+9BF)
_AIL_set_timer_divisor proc far
        retf
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        cmp     word ptr [bp+8],0
        jne     short L09D6
        mov     ax,0D68Dh
        mov     dx,0
        jmp     short L09E4
L09D6:
        mov     ax,2710h
        mov     bx,2E9Ch
        mul     word ptr [bp+8]
        div     bx
        mov     dx,0
L09E4:
        push    dx
        push    ax
        push    word ptr [bp+6]
        call    _AIL_set_timer_period
        add     sp,6
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_set_timer_divisor endp

; seg022_2405_9FC  (+9FC)
_AIL_interrupt_divisor proc far
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     ax,word ptr cs:L0122
        POP_F
        pop     di
        pop     si
        pop     ds
        retf
_AIL_interrupt_divisor endp

; seg022_2405_A10  (+A10)
_AIL_register_driver proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     word ptr cs:L01A8,0
L0A1F:
        mov     si,word ptr cs:L01A8
        shl     si,1
        shl     si,1
        mov     ax,word ptr cs:L0124[si]
        or      ax,word ptr cs:L0126[si]
        je      short L0A46
        inc     word ptr cs:L01A8
        cmp     word ptr cs:L01A8,10h
        jne     L0A1F
        mov     ax,0FFFFh
        jmp     short L0A8E
L0A46:
        les     di,dword ptr [bp+6]
        mov     ax,0FFFFh
        cmp     word ptr es:[di+2],6F43h
        jne     short L0A8E
        cmp     word ptr es:[di+4],7970h
        jne     short L0A8E
        add     di,word ptr es:[di]
        mov     word ptr cs:L0124[si],di
        mov     word ptr cs:L0126[si],es
        push    word ptr cs:L01A8
        push    cs
        call    near ptr _AIL_describe_driver
        add     sp,2
        mov     es,dx
        mov     di,ax
        or      dx,ax
        mov     ax,0FFFFh
        je      short L0A8E
        mov     dx,word ptr es:[di]
        cmp     dx,word ptr cs:L03B8
        ja      short L0A8E
        mov     ax,word ptr cs:L01A8
L0A8E:
        POP_F
        pop     di
        pop     si
_AIL_register_driver endp

; seg022_2405_A97  (+A97)
_AIL_release_driver_handle proc far
        pop     ds
        pop     bp
        retf
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        mov     bx,word ptr [bp+6]
        cmp     bx,10h
        jae     short L0ABC
        shl     bx,1
        shl     bx,1
        mov     word ptr cs:L0124[bx],0
        mov     word ptr cs:L0126[bx],0
L0ABC:
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_release_driver_handle endp

; seg022_2405_AC8  (+AC8)
_AIL_describe_driver proc far
        push    bp
        mov     bp,sp
        push    seg _AIL_interrupt_divisor      ; a callback for the driver
        push    offset _AIL_interrupt_divisor
        push    word ptr [bp+6]
        mov     ax,64h
        call    _call_driver
        add     sp,6
        pop     bp
        retf
_AIL_describe_driver endp

; seg022_2405_AEE  (+AEE)
_AIL_detect_device proc far
        mov     ax,65h
        jmp     _call_driver
_AIL_detect_device endp

; seg022_2405_AF4  (+AF4)
_AIL_init_driver proc far
        push    bp
        mov     bp,sp
        push    ds
        push    si
        push    di
        pushf
        cli
        cmp     word ptr [bp+6],10h
        jb      short L0B05
        jmp     L0B9A
L0B05:
        mov     word ptr cs:L01AE,0FFFFh
        push    word ptr [bp+6]
        call    _AIL_describe_driver
        add     sp,2
        mov     es,dx
        mov     di,ax
        mov     si,word ptr es:[di+14h]
        cmp     si,0FFFFh
        je      short L0B61
        mov     ax,67h
        mov     bx,word ptr [bp+6]
        call    _find_proc
        mov     bx,ax
        or      bx,dx
        je      short L0B61
        mov     es,dx
        mov     bx,ax
        push    es
        push    bx
        call    _AIL_register_timer
        add     sp,4
        mov     bx,word ptr [bp+6]
        shl     bx,1
        mov     word ptr cs:L0164[bx],ax
        mov     word ptr cs:L01AE,ax
        push    0
        push    si
        push    ax
        call    _AIL_set_timer_frequency
        add     sp,6
L0B61:
        push    word ptr [bp+0Eh]
        push    word ptr [bp+0Ch]
        push    word ptr [bp+0Ah]
        push    word ptr [bp+8]
        push    word ptr [bp+6]
        mov     ax,66h
        call    _call_driver
        add     sp,0Ah
        mov     bx,word ptr [bp+6]
        shl     bx,1
        mov     word ptr cs:L0184[bx],1
        cmp     word ptr cs:L01AE,0FFFFh
        je      short L0B9A
        push    word ptr cs:L01AE
        call    _AIL_start_timer
        add     sp,2
L0B9A:
        POP_F
        pop     di
        pop     si
        pop     ds
        pop     bp
        retf
_AIL_init_driver endp

; seg022_2405_BA6  (+BA6)
_AIL_shutdown_driver proc far
        mov     bx,sp
        mov     bx,word ptr ss:[bx+4]
        cmp     bx,10h
        jae     short L0BD8
        shl     bx,1
        mov     dx,0
        xchg    dx,word ptr cs:L0184[bx]
        cmp     dx,0
        je      short L0BD8
        mov     dx,word ptr cs:L0164[bx]
        cmp     dx,0FFFFh
        je      short L0BD2
        push    dx
        call    _AIL_release_timer_handle
        add     sp,2
L0BD2:
        mov     ax,68h
        jmp     _call_driver
L0BD8:
        retf
_AIL_shutdown_driver endp

; seg022_2405_BD9  (+BD9)
_AIL_index_VOC_block proc far
        mov     ax,78h
        jmp     _call_driver
_AIL_index_VOC_block endp

; seg022_2405_BDF  (+BDF)
_AIL_register_sound_buffer proc far
        mov     ax,79h
        jmp     _call_driver
_AIL_register_sound_buffer endp

; seg022_2405_BE5  (+BE5)
_AIL_format_sound_buffer proc far
        mov     ax,86h
        jmp     _call_driver
_AIL_format_sound_buffer endp

; seg022_2405_BEB  (+BEB)
_AIL_sound_buffer_status proc far
        mov     ax,7Ah
        jmp     _call_driver
_AIL_sound_buffer_status endp

; seg022_2405_BF1  (+BF1)
_AIL_play_VOC_file proc far
        mov     ax,7Bh
        jmp     _call_driver
_AIL_play_VOC_file endp

; seg022_2405_BF7  (+BF7)
_AIL_format_VOC_file proc far
        mov     ax,85h
        jmp     _call_driver
_AIL_format_VOC_file endp

; seg022_2405_BFD  (+BFD)
_AIL_VOC_playback_status proc far
        mov     ax,7Ch
        jmp     _call_driver
_AIL_VOC_playback_status endp

; seg022_2405_C03  (+C03)
_AIL_start_digital_playback proc far
        mov     ax,7Dh
        jmp     _call_driver
_AIL_start_digital_playback endp

; seg022_2405_C09  (+C09)
_AIL_stop_digital_playback proc far
        mov     ax,7Eh
        jmp     _call_driver
_AIL_stop_digital_playback endp

; seg022_2405_C0F  (+C0F)
_AIL_pause_digital_playback proc far
        mov     ax,7Fh
        jmp     _call_driver
_AIL_pause_digital_playback endp

; seg022_2405_C15  (+C15)
_AIL_resume_digital_playback proc far
        mov     ax,80h
        jmp     _call_driver
_AIL_resume_digital_playback endp

; seg022_2405_C1B  (+C1B)
_AIL_set_digital_playback_volume proc far
        mov     ax,81h
        jmp     _call_driver
_AIL_set_digital_playback_volume endp

; seg022_2405_C21  (+C21)
_AIL_digital_playback_volume proc far
        mov     ax,82h
        jmp     _call_driver
_AIL_digital_playback_volume endp

; seg022_2405_C27  (+C27)
_AIL_set_digital_playback_panpot proc far
        mov     ax,83h
        jmp     _call_driver
_AIL_set_digital_playback_panpot endp

; seg022_2405_C2D  (+C2D)
_AIL_digital_playback_panpot proc far
        mov     ax,84h
        jmp     _call_driver
_AIL_digital_playback_panpot endp

; seg022_2405_C33  (+C33)
_AIL_state_table_size proc far
        mov     ax,96h
        jmp     _call_driver
_AIL_state_table_size endp

; seg022_2405_C39  (+C39)
_AIL_register_sequence proc far
        mov     ax,97h
        jmp     _call_driver
_AIL_register_sequence endp

; seg022_2405_C3F  (+C3F)
_AIL_release_sequence_handle proc far
        mov     ax,98h
        jmp     _call_driver
_AIL_release_sequence_handle endp

; seg022_2405_C45  (+C45)
_AIL_default_timbre_cache_size proc far
        mov     ax,99h
        jmp     _call_driver
_AIL_default_timbre_cache_size endp

; seg022_2405_C4B  (+C4B)
_AIL_define_timbre_cache proc far
        mov     ax,9Ah
        jmp     _call_driver
_AIL_define_timbre_cache endp

; seg022_2405_C51  (+C51)
_AIL_timbre_request proc far
        mov     ax,9Bh
        jmp     _call_driver
_AIL_timbre_request endp

; FindAndPushAdvEntry_9C_seg022_2405_C57  (+C57)
_AIL_install_timbre proc far
        mov     ax,9Ch
        jmp     _call_driver
_AIL_install_timbre endp

; seg022_2405_C5D  (+C5D)
_AIL_protect_timbre proc far
        mov     ax,9Dh
        jmp     _call_driver
_AIL_protect_timbre endp

; seg022_2405_C63  (+C63)
_AIL_unprotect_timbre proc far
        mov     ax,9Eh
        jmp     _call_driver
_AIL_unprotect_timbre endp

; seg022_2405_C69  (+C69)
_AIL_timbre_status proc far
        mov     ax,9Fh
        jmp     _call_driver
_AIL_timbre_status endp

; seg022_2405_C6F  (+C6F)
_AIL_start_sequence proc far
        mov     ax,0AAh
        jmp     _call_driver
_AIL_start_sequence endp

; seg022_2405_C75  (+C75)
_AIL_stop_sequence proc far
        mov     ax,0ABh
        jmp     _call_driver
_AIL_stop_sequence endp

; seg022_2405_C7B  (+C7B)
_AIL_resume_sequence proc far
        mov     ax,0ADh
        jmp     _call_driver
_AIL_resume_sequence endp

; FindAndPushAdvEntryAE_seg022_2405_C81  (+C81)
_AIL_sequence_status proc far
        mov     ax,0AEh
        jmp     _call_driver
_AIL_sequence_status endp

; seg022_2405_C87  (+C87)
_AIL_relative_volume proc far
        mov     ax,0AFh
        jmp     _call_driver
_AIL_relative_volume endp

; seg022_2405_C8D  (+C8D)
_AIL_relative_tempo proc far
        mov     ax,0B0h
        jmp     _call_driver
_AIL_relative_tempo endp

; seg022_2405_C93  (+C93)
_AIL_set_relative_volume proc far
        mov     ax,0B1h
        jmp     _call_driver
_AIL_set_relative_volume endp

; seg022_2405_C99  (+C99)
_AIL_set_relative_tempo proc far
        mov     ax,0B2h
        jmp     _call_driver
_AIL_set_relative_tempo endp

; seg022_2405_C9F  (+C9F)
_AIL_beat_count proc far
        mov     ax,0B3h
        jmp     _call_driver
_AIL_beat_count endp

; seg022_2405_CA5  (+CA5)
_AIL_measure_count proc far
        mov     ax,0B4h
        jmp     _call_driver
_AIL_measure_count endp

; seg022_2405_CAB  (+CAB)
_AIL_branch_index proc far
        mov     ax,0B5h
        jmp     _call_driver
_AIL_branch_index endp

; seg022_2405_CB1  (+CB1)
_AIL_controller_value proc far
        mov     ax,0B6h
        jmp     _call_driver
_AIL_controller_value endp

; seg022_2405_CB7  (+CB7)
_AIL_set_controller_value proc far
        mov     ax,0B7h
        jmp     _call_driver
_AIL_set_controller_value endp

; seg022_2405_CBD  (+CBD)
_AIL_channel_notes proc far
        mov     ax,0B9h
        jmp     _call_driver
_AIL_channel_notes endp

; seg022_2405_CC3  (+CC3)
_AIL_send_channel_voice_message proc far
        mov     ax,0BAh
        jmp     _call_driver
_AIL_send_channel_voice_message endp

; seg022_2405_CC9  (+CC9)
_AIL_send_sysex_message proc far
        mov     ax,0BBh
        jmp     _call_driver
_AIL_send_sysex_message endp

; seg022_2405_CCF  (+CCF)
_AIL_write_display proc far
        mov     ax,0BCh
        jmp     _call_driver
_AIL_write_display endp

; seg022_2405_CD5  (+CD5)
_AIL_install_callback proc far
        mov     ax,0BDh
        jmp     _call_driver
_AIL_install_callback endp

; seg022_2405_CDB  (+CDB)
_AIL_cancel_callback proc far
        mov     ax,0BEh
        jmp     _call_driver
_AIL_cancel_callback endp

; seg022_2405_CE1  (+CE1)
_AIL_lock_channel proc far
        mov     ax,0BFh
        jmp     _call_driver
_AIL_lock_channel endp

; seg022_2405_CE7  (+CE7)
_AIL_map_sequence_channel proc far
        mov     ax,0C0h
        jmp     _call_driver
_AIL_map_sequence_channel endp

; seg022_2405_CED  (+CED)
_AIL_release_channel proc far
        mov     ax,0C1h
        jmp     _call_driver
_AIL_release_channel endp

; seg022_2405_CF3  (+CF3)
_AIL_true_sequence_channel proc far
        mov     ax,0C2h
        jmp     _call_driver
_AIL_true_sequence_channel endp

        end
