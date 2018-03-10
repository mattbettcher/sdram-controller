#include "../obj_dir/Vsdram.h"
#include "verilated_vcd_c.h"

#include <cinttypes>
#include <iostream>
using namespace std;

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

typedef struct
{
    uint32_t (*get_reset_n)();
    void (*set_reset_n)(uint32_t value);
    uint32_t (*get_clk)();
    void (*set_clk)(uint32_t value);
    uint32_t (*get_inc)();
    void (*set_inc)(uint32_t value);
    uint32_t (*get_count)();
    void (*set_count)(uint32_t value);

    void (*eval)();
    void (*final)();
    void (*trace_dump)(uint64_t time);
} Env;

static Vsdram *top;
static VerilatedVcdC *trace = nullptr;

uint32_t get_reset_n()
{
    return top->reset_n;
}

void set_reset_n(uint32_t value)
{
    top->reset_n = value;
}

uint32_t get_clk()
{
    return top->clk;
}

void set_clk(uint32_t value)
{
    top->clk = value;
}

uint32_t get_inc()
{
    return top->inc;
}

void set_inc(uint32_t value)
{
    top->inc = value;
}

uint32_t get_count()
{
    return top->count;
}

void set_count(uint32_t value)
{
    top->count = value;
}

void eval()
{
    top->eval();
}

void final()
{
    top->final();
}

void trace_dump(uint64_t time)
{
    if (trace)
        trace->dump((vluint64_t)time);
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        cout << "Invalid number of arguments" << endl;
        exit(1);
    }

    auto libName = argv[1];
    auto lib =
#ifdef _WIN32
        LoadLibrary(libName);
#else
        dlopen(libName, RTLD_LAZY);
#endif
    if (!lib)
    {
        cout << "Failed to load library: " << libName << endl;
        exit(1);
    }
    auto run = (int32_t (*)(Env *))
#ifdef _WIN32
        GetProcAddress
#else
        dlsym
#endif
        (lib, "run");
    if (!run)
    {
        cout << "Failed to get run proc address" << endl;
        exit(1);
    }

    top = new Vsdram();

    if (argc == 3)
    {
        Verilated::traceEverOn(true);
        trace = new VerilatedVcdC();
        top->trace(trace, 99); // TODO: What is this param?
        trace->open(argv[2]);
    }

    Env env =
    {
        .get_reset_n = get_reset_n,
        .set_reset_n = set_reset_n,

        .get_clk = get_clk,
        .set_clk = set_clk,

        .get_inc = get_inc,
        .set_inc = set_inc,

        .get_count = get_count,
        .set_count = set_count,

        .eval = eval,
        .final = final,
        .trace_dump = trace_dump,
    };

    auto ret = run(&env);

    if (trace)
    {
        trace->close();
        delete trace;
    }

    delete top;

    return ret;
}
