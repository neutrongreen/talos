from tensorflow.keras import Model
from tensorflow.keras import Model
from keras.layers import Input, Dense
from keras.optimizers import Adam

HIDDEN_SIZE = 32
NUM_LAYERS = 3

def build_actor():
    state_input = Input(shape=(27,))
    advantage = Input(shape=(1,))
    old_prediction = Input(shape=(4,))

    x = Dense(HIDDEN_SIZE, activation='relu')(state_input)
    for i in range(NUM_LAYERS -1):
        x = Dense(HIDDEN_SIZE, activation='relu')(x)
    out_actions = Dense(4, activation='softmax', name='output')(x)

    Model = Model(inputs=[state_input, advantage, old_prediction], outputs=[out_actions])