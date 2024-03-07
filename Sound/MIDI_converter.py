# import mido

# mid = mido.MidiFile("./MIDI/MIDI_sample.mid");

# for i, track in enumerate(mid.tracks):
#     print('Track {}: {}'.format(i, track.name))
#     for msg in track:
#         if ()
#         print("Note: {} for {} ticks", "dfgs", msg.time)
#         print(msg)

import mido
from mido import MidiFile

note_frequencies = {
    0: 8.18,
    1: 8.66,
    2: 9.18,
    3: 9.72,
    4: 10.30,
    5: 10.91,
    6: 11.56,
    7: 12.25,
    8: 12.98,
    9: 13.75,
    10: 14.57,
    11: 15.43,
    12: 16.35,
    13: 17.32,
    14: 18.35,
    15: 19.45,
    16: 20.60,
    17: 21.83,
    18: 23.12,
    19: 24.50,
    20: 25.96,
    21: 27.50,
    22: 29.14,
    23: 30.87,
    24: 32.70,
    25: 34.65,
    26: 36.71,
    27: 38.89,
    28: 41.20,
    29: 43.65,
    30: 46.25,
    31: 49.00,
    32: 51.91,
    33: 55.00,
    34: 58.27,
    35: 61.74,
    36: 65.41,
    37: 69.30,
    38: 73.42,
    39: 77.78,
    40: 82.41,
    41: 87.31,
    42: 92.50,
    43: 98.00,
    44: 103.83,
    45: 110.00,
    46: 116.54,
    47: 123.47,
    48: 130.81,
    49: 138.59,
    50: 146.83,
    51: 155.56,
    52: 164.81,
    53: 174.61,
    54: 185.00,
    55: 196.00,
    56: 207.65,
    57: 220.00,
    58: 233.08,
    59: 246.94,
    60: 261.63,
    61: 277.18,
    62: 293.66,
    63: 311.13,
    64: 329.63,
    65: 349.23,
    66: 369.99,
    67: 392.00,
    68: 415.30,
    69: 440.00,
    70: 466.16,
    71: 493.88,
    72: 523.25,
    73: 554.37,
    74: 587.33,
    75: 622.25,
    76: 659.26,
    77: 698.46,
    78: 739.99,
    79: 783.99,
    80: 830.61,
    81: 880.00,
    82: 932.33,
    83: 987.77,
    84: 1046.50,
    85: 1108.73,
    86: 1174.66,
    87: 1244.51,
    88: 1318.51,
    89: 1396.91,
    90: 1479.98,
    91: 1567.98,
    92: 1661.22,
    93: 1760.00,
    94: 1864.66,
    95: 1975.53,
    96: 2093.00,
    97: 2217.46,
    98: 2349.32,
    99: 2489.02,
    100: 2637.02,
    101: 2793.83,
    102: 2959.96,
    103: 3135.96,
    104: 3322.44,
    105: 3520.00,
    106: 3729.31,
    107: 3951.07,
    108: 4186.01,
    109: 4434.92,
    110: 4698.64,
    111: 4978.03,
    112: 5274.04,
    113: 5587.65,
    114: 5919.91,
    115: 6271.93,
    116: 6644.88,
    117: 7040.00,
    118: 7458.62,
    119: 7902.13,
    120: 8372.02,
    121: 8869.84,
    122: 9397.27,
    123: 9956.06,
    124: 10548.08,
    125: 11175.30,
    126: 11839.82,
    127: 12543.85
}

def get_note_info(track):
    note_info = []
    
    for msg in track:
        if msg.type == 'note_on':
            if msg.velocity != 0:
                note_info.append((note_frequencies[msg.note], msg.time))

    return note_info

def create_c_file(note_info):
    f = open("Music.c", "w")
    f.write("task play_sound()\n{\n")
    f.write("\tint timing_multiplier = 1;\n")
    
    for note in note_info:
        f.write("\tplayTone({},{} * timing_multiplier);\n".format(note[0], note[1]))
    
    f.write("}")
    f.close()

def print_MIDI_data()

def main():
    # Load MIDI file
    mid = MidiFile('./MIDI/test2.mid')

    # Select the track you want to analyze
    track_number = 1  # Change this to the desired track number
    track = mid.tracks[track_number]

    # Get note information
    note_info = get_note_info(track)
    # print(note_info)

    create_c_file(note_info)

if __name__ == "__main__":
    main()
