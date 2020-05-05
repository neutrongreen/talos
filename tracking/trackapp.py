import kivy 
import tracking
import cv2
import csv
import numpy as np
import time 


from kivy.config import Config
Config.set('graphics', 'resizable', False)
Config.set('graphics', 'width', '1280')
Config.set('graphics', 'height', '720')
Config.set('graphics', 'minimum_width', '1280')
Config.set('graphics', 'minimum_height', '720')

kivy.require("1.0.7")



from kivy.app import App
from kivy.uix.image import Image
from kivy.clock import Clock
from kivy.graphics.texture import Texture
from kivy.uix.widget import Widget
from kivy.uix.boxlayout import BoxLayout



class CircularBuffer:
    def __init__(self, size):
        self.size = size
        self.array = []
    def append(self, value):
        if not len(self.array) < self.size:
            del self.array[0]
        self.array.append(value)

    def get_value(self):
        total = 0
        for i in self.array:
            total += i
        return total / (len(self.array) or 1)

class KivyTracker(Image):
    def __init__(self, capture, fps, **kwargs):
        super(KivyTracker, self).__init__(**kwargs)
        self.capture = capture
        self.markerDic = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)
        self.cameraMatrix = np.loadtxt("calb/cameraMatrix.csv", delimiter=",")
        self.distCoeffs = np.loadtxt("calb/distCoeffs.csv", delimiter=",")
        self.app = App.get_running_app()

        self.xa = CircularBuffer(5)
        self.ya = CircularBuffer(5)
        self.za = CircularBuffer(5)

        self.pitcha = CircularBuffer(5)
        self.yawa = CircularBuffer(5)
        self.rolla = CircularBuffer(5)
        self.zero = [0, 0, 0, 0, 0, 0]
        self.file = None
        self.filewriter= None
        Clock.schedule_interval(self.update, 1.0 / fps)
    def togglerecord(self, state):
        if state == "down" and not self.file:
            self.file = open("data/{}_data.csv".format(time.time()), "w+", newline='')
            self.filewriter = csv.writer(self.file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        else:
            self.filewriter = None
            self.file.close()
            self.file = None
    def update(self, dt):
        ret, img = self.capture.read()
        if ret:
            #
            img, rvec, tvec = tracking.track_marker(img, self.markerDic, 0, 20, self.cameraMatrix, self.distCoeffs,)
            #set text icons
            if rvec and tvec:
                self.xa.append(tvec[0])
                self.ya.append(tvec[1])
                self.za.append(tvec[2])

                self.pitcha.append(rvec[0])
                self.yawa.append(tvec[2])
                self.rolla.append(tvec[1])
                #if csv file open append to csv file
                if self.filewriter:
                    self.filewriter.writerow([time.time(), self.xa.get_value() - self.zero[0], self.ya.get_value() - self.zero[1], self.za.get_value() - self.zero[2], self.pitcha.get_value() - self.zero[3], self.yawa.get_value() - self.zero[4], self.rolla.get_value() - self.zero[5]])
                    self.app.root.graph.datax.append(self.xa.get_value() - self.zero[0])

                #print xyz values na pitch yaw roll
            self.app.root.ids.vx.text = "X:{:4.1f}".format(self.xa.get_value() - self.zero[0])
            self.app.root.ids.vy.text = "Y:{:4.1f}".format(self.ya.get_value() - self.zero[1])
            self.app.root.ids.vz.text = "Z:{:4.1f}".format(self.za.get_value() - self.zero[2])

            self.app.root.ids.ax.text = "Pitch:{:4.1f}".format(self.pitcha.get_value() - self.zero[3])
            self.app.root.ids.ay.text = "Yaw:{:4.1f}".format(self.yawa.get_value() - self.zero[4])
            self.app.root.ids.az.text = "Roll:{:4.1f}".format(self.rolla.get_value() - self.zero[5])
            # convert it to texture
            buf1 = cv2.flip(img, 0)
            buf = buf1.tostring()
            image_texture = Texture.create(
                size=(img.shape[1], img.shape[0]), colorfmt='bgr')
            image_texture.blit_buffer(buf, colorfmt='bgr', bufferfmt='ubyte')
            # display image from the texture
            self.texture = image_texture
    def zerovalues(self):
            self.zero[0] = self.xa.get_value()
            self.zero[1] = self.ya.get_value()
            self.zero[2] = self.za.get_value()
            self.zero[3] = self.pitcha.get_value()
            self.zero[4] = self.yawa.get_value()
            self.zero[5] = self.rolla.get_value()
class RootWidget(Widget):
    def __init__(self, **kwargs):
        super(RootWidget, self).__init__(**kwargs)
        self.capture = cv2.VideoCapture(1, cv2.CAP_DSHOW)
        self.capture.set(cv2.CAP_PROP_FRAME_WIDTH,1280)
        self.capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
        self.tracker = KivyTracker(capture=self.capture, fps=30)
        self.tracker.size_hint = (1, 0.5)
        self.ids.v1.add_widget(self.tracker, len(self.ids.v1.children))
        print(self.ids.v1.children)
    def on_stop(self):
        #without this, app will not exit even if the window is closed
        self.capture.release()

class TrackApp(App):
    def build(self):
        widget = RootWidget()
        return widget


Config.set('graphics', 'resizable', False)
Config.set('graphics', 'width', '1280')
Config.set('graphics', 'height', '720')
Config.set('graphics', 'minimum_width', '1280')
Config.set('graphics', 'minimum_height', '720')

if __name__ == "__main__":
    TrackApp().run()
