from tensorflow.keras import Model
from tensorflow.keras import Model
from keras.layers import Input, Dense
from keras.optimizers import Adam
from ppo import *
HIDDEN_SIZE = 128
NUM_LAYERS = 3
INPUT_SIZE = 81
OUTPUT_SIZE = 19
LR = 1e-4  # Lower lr stabilises training greatly

class Actor():
    def build_actor(self):
        state_input = Input(shape=(INPUT_SIZE,))
        advantage = Input(shape=(1,))
        old_prediction = Input(shape=(OUTPUT_SIZE,))

        x = Dense(HIDDEN_SIZE, activation='relu')(state_input)
        for i in range(NUM_LAYERS -1):
            x = Dense(HIDDEN_SIZE, activation='relu')(x)
        out_actions = Dense(OUTPUT_SIZE, activation='softmax', name='output')(x)

        model = Model(inputs=[state_input, advantage, old_prediction], outputs=[out_actions])
        model.compile(optimizer=Adam(lr=LR),
            loss=[proximal_policy_optimization_loss(
                  advantage=advantage,
                        old_prediction=old_prediction)])
        model.summary()
        return model

    def build_critic(self):
        state_input = Input(shape=(INPUT_SIZE,))

        x = Dense(HIDDEN_SIZE, activation='relu')(state_input)
        for i in range(NUM_LAYERS -1):
            x = Dense(HIDDEN_SIZE, activation='relu')(x)
        out_actions = Dense(1, activation='tanh', name='prediction')(x)

        model = Model(inputs=[state_input], outputs=[out_actions])
        model.compile(optimizer=Adam(lr=LR), loss='mse')
        model.summary()
        return model  