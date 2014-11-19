Due to potential patent issues regarding the mp3 format and due to the large size of wav files, we use ogg vorbis format:

play ogg files with ogg123: 

> sudo aptitude install vorbis-tools
> ogg123 file.ogg

convert wav to ogg mono 64kbit:

> for i in *.wav; do oggenc --downmix "$i" -b 64 -o "${i%wav}ogg"; done

use amixer for volume control:

> amixer sget 'PCM',0
> amixer sset 'PCM',0 192,192
> amixer sset 'PCM',0 50%,50%
> amixer sset 'PCM',0 60%


TODO:
  Sound files shall be standardized with repect to volume (or felt volume)
  Sound files shall be repeatable (standardized silence period?)

20130204 TSK

