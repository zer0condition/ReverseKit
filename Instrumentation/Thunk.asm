include ksamd64.inc

extern InstrumentationCallback:proc
EXTERNDEF __imp_RtlCaptureContext:QWORD

.code

InstrumentationCallbackThunk proc
				mov     gs:[2e0h], rsp            ; Win10 TEB InstrumentationCallbackPreviousSp
				mov     gs:[2d8h], r10            ; Win10 TEB InstrumentationCallbackPreviousPc
				mov     r10, rcx                  ; Save original RCX
				sub     rsp, 4d0h                 ; Alloc stack space for CONTEXT structure
				and     rsp, -10h                 ; RSP must be 16 byte aligned before calls
				mov     rcx, rsp
				call    __imp_RtlCaptureContext   ; Save the current register state. RtlCaptureContext does not require shadow space
				sub     rsp, 20h                  ; Shadow space
				call    InstrumentationCallback   ; Call main instrumentation routine
InstrumentationCallbackThunk endp

end