<p align="center">
  <img src="./assets/young1.png">
</p>

## Introduction

This repository is intended to be a resource about various different temperaments that have been developed throughout history to tune keyboard instruments such as the organ, harpsichord, and piano. The fundamental problem that every temperament seeks to solve is how to tune the 12 fixed notes within an octave. Unlike the voice or string instruments, these notes are fixed and cannot practically be adjusted during a performance.

The main output of main.cpp is a table which lists the frequencies of 12 adjacent notes beginning on C, with the starting octave<sup>1</sup> specified by the user. From these frequencies, the deviations from equal temperament, the widths of the intervals compared to their just counterparts<sup>2</sup>, and the associated beats-per-second<sup>3</sup> are also listed. When interpreting intervals, each row corresponds to their lower tone.

The graphical chart shown above is intended to be a loose reproduction of that seen in [this](http://rollingball.com/TemperamentsFrames.htm) guide. We display a summary of Young I temperament, a well (unequal) temperament where the widths of the fifths and thirds differ between keys. The bars signify the deviations, in cents, of these three intervals from their just counterparts. While the minor thirds are usually narrower than just, we opt to plot the absolute value of their deviation for an easier comparison with the major thirds. Observe that the major thirds are close to pure around C major but grow progressively wider as we traverse the circle of fifths, a property that holds for most well temperaments. This notion of "key color" is in stark contrast with the widely used equal temperament, where every interval has the same width, and therefore key color is lost.

<sup>1</sup>We use [scientific pitch notation](https://en.wikipedia.org/wiki/Scientific_pitch_notation) to specify the starting octave. In this notation, C4 is denoted as middle C, and A4 is [concert A](https://en.wikipedia.org/wiki/Concert_pitch). In modern times, A4 = 440 Hz, but other pitch standards have been in use throughout history. Most notably, the modern convention for authentic Baroque performance sets A4 = 415 Hz.

<sup>2</sup>The widths of the intervals are given in units of [musical cents](https://en.wikipedia.org/wiki/Cent_(music)), a logarithmic interval defined such that 100 cents is a semitone in equal temperament. Because the cent is independent of octave, it is a useful unit for theoretical description.

<sup>3</sup>The beats-per-second (bps) of a musical interval is determined by the [beating](https://en.wikipedia.org/wiki/Beat_(acoustics)) of the lowest coinciding harmonic overtones (partials), which a tuner listens to for intonation. For example, the bps of a major third is determined by the 5th partial of the lower tone to the 4th partial of the upper tone, and therefore to tune a pure major third, the tuner listens to the beating occurring two octaves and a major third above the lower tone and adjusts the pitches until the beating disappears. On real pianos, the nonideal nature of the strings contributes to the phenomenon of [inharmonicity](https://en.wikipedia.org/wiki/Inharmonicity), which raises the frequencies of the overtones relative to the fundamental. To set the temperament, tuners will typically start from the middle of the keyboard, where inharmonicity effects are least pronounced. 

## Functionality
Compile using the command ```make```, which creates the binary file ```main```. Two options are available from the command line:

1. ```./main [concertA4] [starting_note] [octave]```, where ```[concertA4]``` is the frequency (Hz) of concert A and ```[octave]``` is the starting octave on C. The command ```[starting_note]``` has two different meanings depending on the tuning system used, as elaborated below.
2. ```./main```, with defaults ```[concertA4] = 440 Hz```, ```[octave] = 4```, and ```[starting_note] = E-flat```.

The abstract class ```TuningSystem``` has four subclasses:

1. ```JustIntonation```. Calculates frequencies based on frequency ratios of small whole numbers, given a starting pitch class. The tritone has a ratio of 45:32. Thus, all ratios are by default based only on the prime numbers 2, 3, and 5. In just intonation, ```[starting_note]``` is the reference pitch class upon which the other 11 pitch classes are tuned to.
2. ```Temperament```. Contains temperaments defined by the tempering of the perfect fifths. Included are the ```pythagorean``` (zero-comma meantone) and ```equal``` (approximately eleventh-comma meantone) temperaments. For meantone temperaments, ```[starting_note]``` refers to the upper note of the wolf fifth, or equivalently, the "flattest" note. The default of ```E-flat``` places the wolf at G-sharp - E-flat. 
3. ```BpsTemperament```. Contains temperaments defined by the bps of the perfect fifths specified in the 4th octave. These are sometimes referred to as equal-beating temperaments, though equal-beating may also refer to temperaments which are designed to have small whole number ratios of the bps of the intervals in major/minor triads. 
4. ```EqualTemperament```. Calculates frequencies based on the nth root of 2. The lowest pitch class is defaulted as ```C```. The way the frequencies are calculated in this class forces ```[starting_note] = C```.


The full list of available temperaments in each class is given as follows:

**JustIntonation**
* ```just5```

**Temperament**
* ```pythagorean```
* ```equal```
* ```meantone3```
* ```meantone4```
* ```meantone6```
* ```meantoneN``` (Nth-comma)
* ```werckmeister3```
* ```kirnberger2```
* ```kirnberger3```
* ```vallotti```
* ```young1```
* ```kellner```

**BpsTemperament**
* ```prelleur```
* ```tansur```
* ```ellis1875```
* ```ellis1885```

**EqualTemperament**
* ```equalN``` (N-EDO)

In the future, we intend to implement an interface for custom temperaments, as well as a method of combining instructions for the perfect fifths with respect to the tempering and bps.

## A Brief History

Our ears are most accustomed to [**just intonation**](https://en.wikipedia.org/wiki/Just_intonation), which relates two tones by frequency ratios of small whole numbers. For example, if a tone is played at 256 Hz with another tone at 512 Hz, the frequency ratio is 2:1 and we perceive this interval as a perfect octave. Other consonant intervals have corresponding frequency ratios, including the perfect fifth (3:2), perfect fourth (4:3), major third (5:4), and minor third (6:5). On the other hand, two tones with a ratio of 211:128 are perceived to not be in tune because of the relatively large numbers in the ratio. Generally speaking, the smaller the numbers in the ratio, the more consonant we perceive the interval to be and the easier it is to tune two notes to that interval.

The most basic tuning system is [**Pythagorean tuning**](https://en.wikipedia.org/wiki/Pythagorean_tuning), which involves only the perfect fifth and the intervals created by stacking these fifths. The perfect fifth consists of 7 semitones, while the octave consists of 12. Because 7 and 12 are coprime, the perfect fifth alone can be used to tune every note within the octave. However, it is a well-known problem that 12 fifths fail to close on itself; the resulting interval "should" be exactly 7 perfect octaves, as can easily be demonstrated on a keyboard, but in fact is slightly too wide. The anomalous interval is known as the [**Pythagorean comma**](https://en.wikipedia.org/wiki/Pythagorean_comma) ((3/2)<sup>12</sup>:2<sup>7</sup>, about 23.46 cents). The presence of this comma means that in Pythagorean tuning, one of the fifths is actually too narrow; it is aptly labeled as a "wolf" fifth. Worse, the tertian harmony ubiquitous in Western music is very dissonant in this tuning system, because the (Pythagorean) major thirds are too wide, while the minor thirds are too narrow.

Central to the music of the Renaissance period was the development of tertian harmony. For the natural evolution of music to proceed in this direction, the major third had to be made just. With this in mind, we introduce the [**syntonic comma**](https://en.wikipedia.org/wiki/Syntonic_comma) (81:80, about 21.5 cents) as the small interval needed to narrow the Pythagorean major third (81:64) to the just major third (5:4). Within the confines of the keyboard, the way to obtain the maximum number of just major thirds is to *temper* all the perfect fifths by a quarter of a syntonic comma. This tuning system is known as [**quarter-comma meantone**](https://en.wikipedia.org/wiki/Quarter-comma_meantone). The advantage of quarter-comma meantone is the eight pure major thirds available, but the drawbacks are many: a very wide "wolf" fifth, four unusable major thirds, and a rather uneven chromatic scale. Although modifications to quarter-comma meantone were made in the 16th and 17th centuries, the problems cannot completely be eliminated. Nevertheless, the temperament excelled for the music of the time.

In response to new developments in classical music, various forms of [**well temperament**](https://en.wikipedia.org/wiki/Well_temperament) were proposed in the 18th and 19th centuries. Common to all well temperaments is the objective of closing the circle of fifths by *unequally* distributing the Pythagorean comma among the perfect fifths. To wit, the wolf fifth is eliminated, and the notion of "key color" emerges with the varying widths of the major and minor thirds among the different keys. Some of the most famous well temperaments are known by names such as [Wreckmeister III](https://en.wikipedia.org/wiki/Werckmeister_temperament) (1691), [Kirnberger III](https://en.wikipedia.org/wiki/Kirnberger_temperament) (1771), and [Young I](https://en.wikipedia.org/wiki/Young_temperament) (1799). In more recent times, a plethora of new well temperaments have been made by scholars and enthusiasts.

The most commonly used temperament today is 12-tone [**equal temperament**](https://en.wikipedia.org/wiki/Equal_temperament), where every perfect fifth is tempered by 1/12th of a Pythagorean comma. Every interval sounds the same, and one can freely modulate to any key without worry. Equal temperament is also equivalent to that tuning system where every semitone has a ratio of 2<sup>1/12</sup>:1. Because the 12th root of 2 is irrational, no interval besides the octave is perfectly in tune. However, in practice, the perfect fifths and fourths are very close to pure, and owing to the overtone-poor acoustics of the piano, listeners have learned to tolerate the wide major thirds and narrow minor thirds found in this temperament. Other equal temperaments which divide the octave differently than 12ET have also been used in contemporary music, the most famous of which are 19ET (approximates third-comma meantone), 31ET (approximates quarter-comma meantone), and 53ET (approximates Pythagorean tuning).
