
bool is_in(std::vector<int> set, int x){
    for (int i = 0; i < set.size(); i++)
    {
        if(set[i] == x) return true;
    }
    return false;
}
