```python
import mido

INPUT_MIDI = "Bohemian_Rhapsody.mid"
OUTPUT_HEADER = "song_data.h"

# Cakewalk 화면의 트랙 번호 (보통 0부터 시작하므로 Track 4는 3 또는 4)
# 아래 스크립트를 먼저 실행해 트랙 목록을 보고 보컬 트랙 번호를 넣으세요.
TARGET_TRACK = 3  

MIN_SAFE_NOTE = 59  # B3
MAX_SAFE_NOTE = 84  # C6

def midi_to_hz(note):
    if note <= 0:
        return 0
    return int(round(440.0 * (2.0 ** ((note - 69.0) / 12.0))))

def fold_to_safe(note):
    while note < MIN_SAFE_NOTE:
        note += 12
    while note > MAX_SAFE_NOTE:
        note -= 12
    return note

mid = mido.MidiFile(INPUT_MIDI)

# 1. 트랙 이름 먼저 출력해서 몇 번이 보컬인지 확인
print("=== 트랙 목록 ===")
for idx, trk in enumerate(mid.tracks):
    print(f"Track {idx}: {trk.name} (이벤트 {len(trk)}개)")

# 2. 지정한 단일 트랙만 추출
track = mid.tracks[TARGET_TRACK]
print(f"\n>> Track {TARGET_TRACK} ({track.name}) 추출 시작...")

segments = []
tempo = 500000

for msg in track:
    if msg.type == 'set_tempo':
        tempo = msg.tempo
    
    # 틱 -> 밀리초 변환
    dur_ms = int(round(mido.tick2second(msg.time, mid.ticks_per_beat, tempo) * 1000))
    
    if dur_ms > 0:
        if segments:
            segments[-1][1] += dur_ms
        else:
            segments.append([0, dur_ms])

    if msg.type == 'note_on' and msg.velocity > 0:
        hz = midi_to_hz(fold_to_safe(msg.note))
        segments.append([hz, 0])
    elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
        segments.append([0, 0])

# 빈 틈 및 중복 정리
clean_segments = []
for freq, dur in segments:
    if dur <= 0:
        continue
    if clean_segments and clean_segments[-1][0] == freq:
        clean_segments[-1][1] += dur
    else:
        clean_segments.append([freq, dur])

with open(OUTPUT_HEADER, "w", encoding="utf-8") as f:
    f.write("#pragma once\n#include <Arduino.h>\n\n")
    f.write("struct NoteEvent {\n    uint16_t freq;\n    uint16_t duration;\n};\n\n")
    f.write(f"const NoteEvent PROGMEM SONG_DATA[{len(clean_segments)}] = {{\n")
    for freq, dur in clean_segments:
        f.write(f"    {{{freq:4d}, {dur:4d}}},\n")
    f.write("};\n\n")
    f.write(f"const uint16_t SONG_LENGTH = {len(clean_segments)};\n")

print(f"완료: {OUTPUT_HEADER} 생성 (순수 단일 트랙 멜로디)")
```