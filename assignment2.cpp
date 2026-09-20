#include<iostream>
#include<bits/stdc++.h>
using namespace std;

enum position{
    left,
    right,
};
class state{
    public:
    position person;
    position wolf;
    position goat;
    position cabbage;
};
enum action{
    start,
    move_wolf,
    move_goat,
    move_cabbage,
    move_person
};
class node{
    public:
    state current_state;
    action action_taken;

    node* parent;
    node* child;
    node* sibling;
    node(){
        parent=NULL;
        child=NULL;
        sibling=NULL;
        action_taken=action::start;
    }
};
state start_state(){
    state temp;
    temp.person=position::left;
    temp.wolf=position::left;
    temp.goat=position::left;
    temp.cabbage=position::left;
    return temp;
};
state goal_state(){
    state temp;
    temp.person=position::right;
    temp.wolf=position::right;
    temp.goat=position::right;
    temp.cabbage=position::right;
    return temp;
};
bool same_state(state current,state stateB){                    // for comparing two states
    if(current.cabbage==stateB.cabbage &&
    current.goat == stateB.goat &&
    current.person==stateB.person && 
    current.wolf == stateB.wolf){
        return true;
    }
    return false;
}
bool is_goal(node*current){
    state goal=goal_state();
    return same_state(current->current_state,goal);
}
node* create_node(){    // start with this node where eveyone is on left
    node* start_node=new node();
    start_node->current_state=start_state();
    return start_node;
}
state edit_state(state parent, action change)
{
    // Person always moves
    parent.person = (parent.person == position::left)
                    ? position::right
                    : position::left;

    // Move the selected item with the person
    switch(change)
    {
        case move_wolf:
            parent.wolf = (parent.wolf == position::left)
                          ? position::right
                          : position::left;
            break;

        case move_goat:
            parent.goat = (parent.goat == position::left)
                          ? position::right
                          : position::left;
            break;

        case move_cabbage:
            parent.cabbage = (parent.cabbage == position::left)
                             ? position::right
                             : position::left;
            break;

        case move_person:
            // Person has already been moved.
            break;
    }

    return parent;
}
bool canMove(const state& s, action a)
{
    switch(a)
    {
        case move_person:
            return true;

        case move_wolf:
            return s.wolf == s.person;

        case move_goat:
            return s.goat == s.person;

        case move_cabbage:
            return s.cabbage == s.person;

        default:
            return false;
    }
}
bool isValidState(const state& s)
{
    // Wolf eats goat
    if (s.wolf == s.goat && s.person != s.wolf)
        return false;

    // Goat eats cabbage
    if (s.goat == s.cabbage && s.person != s.goat)
        return false;

    return true;
}
node* addchild(node* parent)
{
    node* firstChild = nullptr;
    node* lastChild = nullptr;

    action actions[4] = {
        move_person,
        move_cabbage,
        move_goat,
        move_wolf
    };

    for (int i = 0; i < 4; i++)
    {
        action currentAction = actions[i];

        if (!canMove(parent->current_state, currentAction)){
            continue;
        }

        // Create the state after performing the action
        state newState = edit_state(
            parent->current_state,
            currentAction
        );

        // Only construct the branch if the state is valid
        if (!isValidState(newState))
            continue;

        node* child = new node();

        child->current_state = newState;
        child->action_taken = currentAction;
        child->parent = parent;
        child->child = nullptr;
        child->sibling = nullptr;

        // First valid child
        if (firstChild == nullptr)
        {
            firstChild = child;
            lastChild = child;
        }
        else
        {
            lastChild->sibling = child;
            lastChild = child;
        }
    }

    // Connect first child to parent
    parent->child = firstChild;

    return firstChild;
}
int main(){
    cout<<"Hello World"<<endl;
    return 0;
}
