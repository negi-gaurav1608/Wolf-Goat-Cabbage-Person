#include<iostream>

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
state edit_state(const state& current, action change)
{
    state next = current;

    // Person always moves.
    // Since the boat is with the person,
    // this also represents the boat changing sides.
    next.person = (next.person == position::left)
                  ? position::right
                  : position::left;

    // Move the selected item with the person.
    switch(change)
    {
        case move_wolf:
            next.wolf = (next.wolf == position::left)
                        ? position::right
                        : position::left;
            break;

        case move_goat:
            next.goat = (next.goat == position::left)
                        ? position::right
                        : position::left;
            break;

        case move_cabbage:
            next.cabbage = (next.cabbage == position::left)
                           ? position::right
                           : position::left;
            break;

        case move_person:
            // Person travels alone.
            break;

        default:
            break;
    }

    return next;
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
bool stateInPath(node* current, const state& s)
{
    node* temp = current;

    while (temp != nullptr)
    {
        if (same_state(temp->current_state, s))
        {
            return true;
        }

        temp = temp->parent;
    }

    return false;
}
node* addchild(node* parent)
{
    node* firstChild = nullptr;
    node* lastChild = nullptr;

    // Four possible boat actions
    action actions[4] = {
        move_person,
        move_wolf,
        move_goat,
        move_cabbage
    };

    for (int i = 0; i < 4; i++)
    {
        action currentAction = actions[i];

        // Check whether the action is possible
        if (!canMove(parent->current_state, currentAction))
        {
            continue;
        }

        // Apply the action
        state newState = edit_state(
            parent->current_state,
            currentAction
        );

        // Reject unsafe states
        if (!isValidState(newState))
        {
            continue;
        }

        // Reject states already present
        // on the current path
        if (stateInPath(parent, newState))
        {
            continue;
        }

        // Create child
        node* child = new node();

        child->current_state = newState;
        child->action_taken = currentAction;
        child->parent = parent;
        child->child = nullptr;
        child->sibling = nullptr;

        // Add child to linked list
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

    parent->child = firstChild;

    return firstChild;
}
string actionName(action a)
{
    switch(a)
    {
        case move_person:
            return "Person crosses alone";

        case move_wolf:
            return "Person takes wolf";

        case move_goat:
            return "Person takes goat";

        case move_cabbage:
            return "Person takes cabbage";

        default:
            return "Start";
    }
}
void printPath(node* current)
{
    if (current == nullptr)
    {
        return;
    }

    // Print the parent first so that
    // the path appears from start to goal.
    if (current->parent != nullptr)
    {
        printPath(current->parent);

        cout << "    "
             << actionName(current->action_taken)
             << " -> ";
    }
    else
    {
        cout << "    Start -> ";
    }

    cout << "("
         << (current->current_state.person == position::left ? "L" : "R")
         << ", "
         << (current->current_state.wolf == position::left ? "L" : "R")
         << ", "
         << (current->current_state.goat == position::left ? "L" : "R")
         << ", "
         << (current->current_state.cabbage == position::left ? "L" : "R")
         << ")"
         << endl;
}
int solutionCount = 0;

void DFS(node* current)
{
    if (current == nullptr)
    {
        return;
    }

    // Check whether we have reached the goal.
    if (is_goal(current))
    {
        solutionCount++;

        cout << "\n============================" << endl;
        cout << "Solution " << solutionCount << endl;
        cout << "============================" << endl;

        printPath(current);

        return;
    }

    // Generate valid successor states.
    addchild(current);

    // Explore every child using DFS.
    node* child = current->child;

    while (child != nullptr)
    {
        DFS(child);

        child = child->sibling;
    }
}
int main()
{
    node* root = create_node();

    cout << "Starting DFS..." << endl;

    DFS(root);

    cout << "\n============================" << endl;
    cout << "Total solutions: "
         << solutionCount << endl;
    cout << "============================" << endl;

    return 0;
}
