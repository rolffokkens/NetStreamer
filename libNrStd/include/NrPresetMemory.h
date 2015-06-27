#ifndef H_NR_PRESET_MEMORY
#define H_NR_PRESET_MEMORY

class NrPresetMemory {
private:
    int Size;
    int *Data;
public:
    NrPresetMemory (int Size);
    ~NrPresetMemory (void);

    void SetVal (int Index, int  Val);
    void GetVal (int Index, int &Val);
};

#endif
