; thanks to https://github.com/Deputation/instrumentation_callbacks


EXTERN CallbackRoutine:NEAR
EXTERNDEF __imp_RtlCaptureContext:QWORD

.code

Bridge PROC	
    ; credits to https://gist.github.com/esoterix/df38008568c50d4f83123e3a90b62ebb
    mov gs:[2e0h], rsp ; Win10 TEB InstrumentationCallbackPreviousSp
    mov gs:[2d8h], r10 ; Win10 TEB InstrumentationCallbackPreviousPc
	
	mov r10, rcx ; save rcx
	sub rsp, 4d0h ; CONTEXT structure size
	and rsp, -10h ; align rsp
	mov rcx, rsp ; parameters are fun
	call __imp_RtlCaptureContext ; capture the thread's context
	
	sub rsp, 20h ; shadow stack space
	call CallbackRoutine ; call our callback which will restore context and go back to where we want
	
	int 3 ; we should not be here.
Bridge ENDP

END