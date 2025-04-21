#define MAX_RESOURCES 5
// available_resources is the shared data involved in the race condition
int available_resources =
    MAX_RESOURCES; // (a) Data involved in the race condition

int decrease_count(int count) {
    // (b) Race condition can occur here: checking and updating
    // available_resources is not atomic
    if (available_resources < count)
        return -1;
    else {
        available_resources -=
            count; // (b) Race condition: modification of shared data
        return 0;
    }
}

int increase_count(int count) {
    available_resources +=
        count; // (b) Race condition: modification of shared data
    return 0;
}
