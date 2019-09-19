import multiprocessing

from playsound import playsound


class MusicThread(multiprocessing.Process):
    def __init__(self, music_id):
        super().__init__()
        self.daemon = True
        if music_id == 1:
            self.song = "Cell Lab Challenge Song"
        elif music_id == 2:
            self.song = "Cell Lab Experiment Song"

    def run(self):
        while True:
            playsound("Resources/" + self.song + ".wav")
