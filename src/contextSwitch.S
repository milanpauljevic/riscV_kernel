.global _ZN3TCB13contextSwitchEPNS_7ContextES1_
.type _ZN3TCB13contextSwitchEPNS_7ContextES1_, @function
_ZN3TCB13contextSwitchEPNS_7ContextES1_:
    #a0=&oldContext, a1=&runningContext
    #cuvanje starog konteksta
    sd ra, 0*8(a0)
    sd sp, 1*8(a0)
    #novi kontekst
    ld ra, 0*8(a1)
    ld sp, 1*8(a1)

    ret