#include <dos.h>   /* for delay(), outp() */
#include <conio.h> /* for getch() */

#define OPL_ADDR 0x388
#define OPL_DATA 0x389

#define SINE     0
#define SQUARE   1
#define TRIANGLE 2
#define SAWTOOTH 3

void outOPL(unsigned char reg, unsigned char val) {
    outp(OPL_ADDR, reg);
    delay(1);
    outp(OPL_DATA, val);
}

/* Approximates F-number and block for a given frequency (Hz) */
void freqToFnum(unsigned int hz, unsigned char *fnum_l, unsigned char *fnum_h) {
    unsigned long f = ((unsigned long)hz << 20) / 49716UL;
    unsigned char block = 0;

    while (f > 1023 && block < 7) {
        f >>= 1;
        block++;
    }

    *fnum_l = (unsigned char)(f & 0xFF);
    *fnum_h = (unsigned char)(((f >> 8) & 0x03) | (block << 2));
}

void setupOPLChannel(unsigned char chan, unsigned char waveform) {
    unsigned char op1 = chan;
    unsigned char op2 = chan + 3;
    unsigned char waveSel = 0x00;

    if (waveform == SQUARE) waveSel = 0x01;
    else if (waveform == TRIANGLE) waveSel = 0x02;
    else if (waveform == SAWTOOTH) waveSel = 0x03;

    /* Enable OPL3 mode */
    outOPL(0x105, 0x01);

    /* Enable waveform select */
    outOPL(0x01, 0x20);

    /* Set waveform */
    outOPL(0xE0 + op1, waveSel);
    outOPL(0xE0 + op2, waveSel);

    /* Operator setup */
    outOPL(0x20 + op1, 0x01); /* Tremolo/vibrato/mult */
    outOPL(0x20 + op2, 0x01);
    outOPL(0x40 + op1, 0x10); /* Volume (lower = louder) */
    outOPL(0x40 + op2, 0x10);
    outOPL(0x60 + op1, 0xF0); /* Attack/decay */
    outOPL(0x60 + op2, 0xF0);
    outOPL(0x80 + op1, 0x77); /* Sustain/release */
    outOPL(0x80 + op2, 0x77);
    outOPL(0xC0 + chan, 0x00); /* Feedback/FM connection */
}

void playTone(unsigned char waveform, unsigned int freq, unsigned int durationMs) {
    unsigned char fnum_l, fnum_h;
    unsigned char chan = 0;

    freqToFnum(freq, &fnum_l, &fnum_h);
    setupOPLChannel(chan, waveform);

    outOPL(0xA0 + chan, fnum_l);
    outOPL(0xB0 + chan, 0x20 | fnum_h); /* Key-on */

    delay(durationMs);

    outOPL(0xB0 + chan, fnum_h); /* Key-off */
}

int main() {
    playTone(SINE, 440, 500);     /* A4 */
    playTone(SQUARE, 523, 500);   /* C5 */
    playTone(TRIANGLE, 659, 500); /* E5 */
    playTone(SAWTOOTH, 784, 500); /* G5 */

    getch(); /* Wait for keypress before exit */
    return 0;
}
