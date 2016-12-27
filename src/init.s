.export __STARTUP__:absolute=1
.include "zeropage.inc"

;; ines header
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "HEADER"
.byte $4e,$45,$53,$1a ;;file indicator
.byte 1 ;;size of PRG ROM in 16KiBs
.byte 1 ;;size of CHR ROM in 8KiBs
.byte 0 ;;flags 6: MMC5 mapper
.byte 0 ;;flags 7
.res 8,0 ;;fill bytes

;; initialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "CODE"

start:
	sei
	cld
	ldx #$40
	stx $4017
	ldx #$FF
	txs
	inx
	stx $2000
	stx $2001
	stx $4010
   @loop:
	lda $2002
	bpl @loop
	lda #$00
	
	bit $2002
   @vwait1:
	bit $2002
	bpl @vwait1
   @vwait2:
	bit $2002
	bpl @vwait2

clearram:
	sta $00, x
	sta $0100, x
	sta $0200, x
	sta $0300, x
	sta $0400, x
	sta $0500, x
	sta $0600, x
	sta $0700, x
	inx
	bne clearram
   @loop:
	lda $2002
	bpl @loop

initGraphics:
	jsr _clearSprites
	lda #$00
	sta $2003
	lda #$02
	sta $4014

postInit:
   .import __STACK_START__, __STACK_SIZE__
	lda #<(__STACK_START__+__STACK_SIZE__)
    sta	sp
    lda	#>(__STACK_START__+__STACK_SIZE__)
    sta	sp+1 ;set pointer
	
   .import initlib, copydata
	jsr	copydata
	jsr	initlib
	
	lda $2002
		
   .import _main
	jmp _main

_clearSprites:
	ldy #$40
	ldx #$00
	lda #$FF
   @loop:
	sta $200, X
	inx
	inx
	inx
	inx
	dey
	bne @loop
	rts
	

;; graphics
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "ZEROPAGE"
.export _ppuNMITicks
_ppuNMITicks: .res $1

tmp0 : .res $2

.export _ppuBgPalette
_ppuBgPalette: .res $4
.export _ppuSprPalette
_ppuSprPalette: .res $3

NTBufferState: .res $1

.segment "BSS"
.export _ppuNTBuffer
_ppuNTBuffer: .res $400

.macro moveNTBuffer hiaddr, loaddr, bufoff, bufsiz
	lda hiaddr
	sta $2006
	lda loaddr
	sta $2006
	ldx $0
	:	lda _ppuNTBuffer+bufoff, X
		sta $2007
		inx
		cpx bufsiz
		bne :-
.endmacro

.segment "CODE"
nmiHandler:
	pha
	txa
	pha
	tya
	pha
	
	inc _ppuNMITicks
	
   @updateNT:
	ldx NTBufferState
   @ntstate0:
	cpx #$0
	bne @ntstate1
		moveNTBuffer #$20, #$00, $00, #$90
		jmp @nextntstate
   @ntstate1:
  	cpx #$1
	bne @ntstate2
		moveNTBuffer #$20, #$90, $90, #$90
		jmp @nextntstate
   @ntstate2:
  	cpx #$2
	bne @ntstate3
		moveNTBuffer #$21, #$20, $120, #$90
		jmp @nextntstate
   @ntstate3:
 	cpx #$3
	bne @ntstate4
		moveNTBuffer #$21, #$B0, $1B0, #$90
		jmp @nextntstate
   @ntstate4:
 	cpx #$4
	bne @ntstate5
		moveNTBuffer #$22, #$40, $240, #$90
		jmp @nextntstate
   @ntstate5:
 	cpx #$5
	bne @ntstate6
		moveNTBuffer #$22, #$D0, $2D0, #$98
		jmp @nextntstate
   @ntstate6:
 	cpx #$6
	bne @ntstate7
		moveNTBuffer #$23, #$68, $368, #$98
		jmp @nextntstate
   @ntstate7:
 	cpx #$7
	bne @nextntstate
		lda #$2
		sta $4014

   @nextntstate:
	inc NTBufferState
	ldx NTBufferState
	cpx #$8
	bcc @resetscroll
		ldx #$0
		stx NTBufferState
	
   @writebgpalette:
	lda #$3F
	sta $2006
	lda #$00
	sta $2006
	lda _ppuBgPalette
	sta $2007
	lda _ppuBgPalette+1
	sta $2007
	lda _ppuBgPalette+2
	sta $2007
	lda _ppuBgPalette+3
	sta $2007
	
	@writesprpalette:
	lda #$3F
	sta $2006
	lda #$11
	sta $2006
	lda _ppuSprPalette
	sta $2007
	lda _ppuSprPalette+1
	sta $2007
	lda _ppuSprPalette+2
	sta $2007
	
   @resetscroll:
	lda #$0
	sta $2006
	sta $2006
	sta $2005
	sta $2005
	
	pla
	tay
	pla
	tax
	pla
	rti


;; irq vectors	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

irqHandler:
    rti
	
.segment "VECTORS"
.word nmiHandler	;0xFFFA: vblank nmi
.word start	;0xFFFC: reset
.word irqHandler	;0xFFFE irq/brk

;; sound
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "CODE"
.include "famitone3.s"

.export _sndTick
_sndTick:
	jsr FamiToneUpdate
	rts

;;
;; argument 0 (A/X): 16bit pointer to music data
.export _sndPlay
_sndPlay:
	sta tmp0
	txa
	tay
	ldx tmp0
	lda #$1
	jsr FamiToneInit
		
	lda #$0
	jsr FamiToneMusicPlay
	rts
	
.export _sndStop
_sndStop:
	jsr FamiToneMusicStop
	rts
	
;;
;; argument 0 (A): sfx index
.export _sndPlaySfx
_sndPlaySfx:
	sta tmp0
	txa
	tay
	ldx tmp0
	jsr FamiToneSfxInit
		
	ldx #$0
	lda #$1
	jsr FamiToneSfxPlay
	rts

.segment "RODATA"
.export _bgmIntro
_bgmIntro:
   .include "../res/bgmintro.s"
   
.export _bgmBegin
_bgmBegin:
   .include "../res/bgmbegin.s"
  
.export _sfxStart  
_sfxStart:
   .include "../res/sfxstart.s"
   
.export _sfxText 
_sfxText:
   .include "../res/sfxtext.s"
  
;; graphics
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "CHR"
.incbin "../res/gfx.bin"
