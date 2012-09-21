/*
 * FSM.h
 *
 *  Created on: 21 Σεπ 2011
 *      Author: manolis
 */

#ifndef RSM_H_
#define RSM_H_

#include <assert.h>
#include "Value.h"

#define DECLARE_STATE_COPY(StateClass) StateClass* copy () const { return new StateClass (); };

namespace base {

  /**
   * @brief Pure virtual template class for handling state machines with sub-states
   *
   * Each state machine is actually the HEAD of its state-chain, therefore each
   * time it enters a new state before the end of the previous one, the new
   * state is added to the tail of the state-chain. Each time a sub-state enters
   * a new state it is added to the tail of the chain it belongs. Thus, a
   * succession of states is formed in a serial manner, rather than a hierarchy
   * in a tree-like fashion.
   */
  template <class T>
  class State {
  public:
    State() {}
    virtual ~State() {}

    /**
     * Enters a new sub-state. It esentially adds the state to the tail of the chain
     * keeping the previous one.
     *
     * @param state The new state to add.
     */
    void enterState(const State<T>& state) {
      /**
       * Description of algorithm:
       *  If we are already in a state:
       *    if it is active, add the state to its tail
       *    else, get rid of the state we're in and replace it with the new one.
       *  If we're not in a state:
       *    Put the new state in the HEAD of the (empty) chain
       */
      if (!isActive() && getParentState())
        getParentState()->enterState(state);
      else if (getState()) {
        if (getState()->isActive())
          getState()->enterState(state);
        else {
          delete m_subState;
          createSubState(this, state);
        }
      }
      else
        createSubState(this, state);
    }

    void endState() {
      if (getState())
        getState()->endState();
      finalizeState();
    }

    void execute() {
      preUpdateState();
      if (getState()) {
        if (getState()->isActive())
          getState()->execute();
        else {
          delete m_subState;
          m_subState = 0;
        }
      }
      postUpdateState();
    }

    virtual void init() = 0;
    virtual void finalize() = 0;
    virtual void preUpdate() = 0;
    virtual void postUpdate() = 0;

    void initState() {
      m_active = true;
      init();
    }
    void finalizeState() {
      m_active = false;
      finalize();
    }
    void preUpdateState() {
      preUpdate();
    }
    void postUpdateState() {
      postUpdate();
    }

    virtual const Char* getName() const { return ""; }
    State<T>* getState() { return m_subState; };
    T* getObj() const { return m_obj; };
    State<T>* getParentState() { return m_parentState; };
    virtual State<T>* copy () const { return new State<T> (); }
    bool isActive() { return m_active; };

  private:
    State<T>* m_subState;
    State<T>* m_parentState;
    T*      m_obj;
    bool    m_active;
  };

  template <class C> void createSubState(State<C>* state, const State<C>& subState) {
    State<C>* newState = subState.copy();
    newState->setObj(state->getObj());
    state->setState(newState);
    newState->setParentState(state);
    newState->initState();
  }
}

#endif /* RSM_H_ */
