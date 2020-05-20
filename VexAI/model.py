from tensorflow.keras import Model
from tensorflow.keras import Model
from tensorflow.keras.layers import Input, Dense
from tensorflow.keras .optimizers import Adam
from ppo import *
from Envrioment import *


HIDDEN_SIZE = 128
NUM_LAYERS = 3
INPUT_SIZE = 81
OUTPUT_SIZE = 19
LR = 1e-4  # Lower lr stabilises training greatly

LOSS_CLIPPING = 0.2 # Only implemented clipping for the surrogate loss, paper said it was best
ENTROPY_LOSS = 5e-3

DUMMY_ACTION, DUMMY_VALUE = np.zeros((1, 19)), np.zeros((1, 1))

class Actor():
    def __init__(self):
        self.match = Match()
        self.actor = self.build_actor()
        self.critc = self.build_critic()
        self.observation, _ = self.match.get_state()
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
                        old_prediction=old_prediction,
                            ENTROPY_LOSS=ENTROPY_LOSS,
                                LOSS_CLIPPING=LOSS_CLIPPING)])
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

    def get_action(self):
        print(self.observation)
        p = self.actor.predict([self.observation.reshape(1, 81), DUMMY_VALUE, DUMMY_ACTION])
        print(p)
    
ai = Actor()
ai.get_action()
