import numpy as np
import tensorflow as tf
from tensorflow.keras import Model
from tensorflow.keras.layers import Input, Dense
from tensorflow.keras.optimizers import Adam
from ppo import proximal_policy_optimization_loss
from Envrioment import *
HIDDEN_SIZE = 32
NUM_LAYERS = 3
LR = 1e-4  # Lower lr stabilises training greatly

LOSS_CLIPPING = 0.2 # Only implemented clipping for the surrogate loss, paper said it was best
ENTROPY_LOSS = 5e-3
STEPS_IN_MATCH = 200
DUMMY_ACTION, DUMMY_VALUE = np.zeros((1, 19)), np.zeros((1, 1))

env = Match()
val = False
def build_actor():
    state_input = Input(shape=(27,))
    advantage = Input(shape=(1,))
    old_prediction = Input(shape=(4,))

    x = Dense(HIDDEN_SIZE, activation='relu')(state_input)
    for _ in range(NUM_LAYERS - 1):
        x = Dense(HIDDEN_SIZE, activation='relu')(x)
    out_actions = Dense(19, activation='softmax', name='output')(x)

    model = Model(inputs=[state_input, advantage, old_prediction], outputs=[out_actions])
    model.compile(optimizer=Adam(lr=LR),
                loss=[proximal_policy_optimization_loss(
                    advantage=advantage,
                    old_prediction=old_prediction, LOSS_CLIPPING=LOSS_CLIPPING, ENTROPY_LOSS=LOSS_CLIPPING)])
    model.summary()
    return model

def build_critic():
    state_input = Input(shape=(27,))
    x = Dense(HIDDEN_SIZE, activation='relu')(state_input)
    for _ in range(NUM_LAYERS - 1):
        x = Dense(HIDDEN_SIZE, activation='relu')(x)
    out_actions = Dense(1, activation='tanh', name='predictions')(x)
    model = Model(inputs=[state_input], outputs=[out_actions])
    model.compile(optimizer=Adam(lr=LR),loss='mse')
    model.summary()
    return model

def get_action(model, critic, envro):
    field = envro.env
    inputs = np.array(field).flatten()
    p = model.predict([tf.convert_to_tensor(inputs), DUMMY_ACTION, DUMMY_VALUE])
    if val is False:
        action = np.random.choice(19, p=np.nan_to_num(p[0]))
    else:
        action = np.argmax(p[0])
    action_matrix = np.zeros(19)
    action_matrix[action] = 1
    return action, action_matrix, p
critic = build_critic()
actor = build_actor()
