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
    
    // clk
    uint32_t (*get_clk)();
    void (*set_clk)(uint32_t value);
    // bank address
    uint32_t (*get_ba)();
    void (*set_ba)(uint32_t value);
    // cas
    uint32_t (*get_cas_n)();
    void (*set_cas_n)(uint32_t value);
    // cs_n
    uint32_t (*get_cs_n)();
    void (*set_cs_n)(uint32_t value);
    // dqmh
    uint32_t (*get_dqmh)();
    void (*set_dqmh)(uint32_t value);
    // dqml
    uint32_t (*get_dqml)();
    void (*set_dqml)(uint32_t value);
    // ras
    uint32_t (*get_ras_n)();
    void (*set_ras_n)(uint32_t value);
    // we_n
    uint32_t (*get_we_n)();
    void (*set_we_n)(uint32_t value);
    // reset
    uint32_t (*get_rst_n)();
    void (*set_rst_n)(uint32_t value);
    // address
    uint32_t (*get_addr)();
    void (*set_addr)(uint32_t value);

    void (*eval)();
    void (*final)();
    void (*trace_dump)(uint64_t time);
} Env;

static Vsdram *top;
static VerilatedVcdC *trace = nullptr;

uint32_t get_clk()
{
    return top->clk;
}

void set_clk(uint32_t value)
{
    top->clk = value;
}

uint32_t get_ba()
{
    return top->ba;
}

void set_ba(uint32_t value)
{
    top->ba = value;
}

uint32_t get_cas_n()
{
    return top->cas_n;
}

void set_cas_n(uint32_t value)
{
    top->cas_n = value;
}

uint32_t get_cs_n()
{
    return top->cs_n;
}

void set_cs_n(uint32_t value)
{
    top->cs_n = value;
}

uint32_t get_dqmh()
{
    return top->dqmh;
}

void set_dqmh(uint32_t value)
{
    top->dqmh = value;
}

uint32_t get_dqml()
{
    return top->dqml;
}

void set_dqml(uint32_t value)
{
    top->dqml = value;
}

uint32_t get_ras_n()
{
    return top->ras_n;
}

void set_ras_n(uint32_t value)
{
    top->ras_n = value;
}

uint32_t get_we_n()
{
    return top->we_n;
}

void set_we_n(uint32_t value)
{
    top->we_n = value;
}

uint32_t get_rst_n()
{
    return top->rst_n;
}

void set_rst_n(uint32_t value)
{
    top->rst_n = value;
}

uint32_t get_addr()
{
    return top->addr;
}

void set_addr(uint32_t value)
{
    top->addr = value;
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
        .get_clk = get_clk,
        .set_clk = set_clk,
        
        .get_ba = get_ba,
        .set_ba = set_ba,

        .get_cas_n = get_cas_n,
        .set_cas_n = set_cas_n,

        .get_cs_n = get_cs_n,
        .set_cs_n = set_cs_n,

        .get_dqmh = get_dqmh,
        .set_dqmh = set_dqmh,

        .get_dqml = get_dqml,
        .set_dqml = set_dqml,

        .get_ras_n = get_ras_n,
        .set_ras_n = set_ras_n,

        .get_we_n = get_we_n,
        .set_we_n = set_we_n,

        .get_rst_n = get_rst_n,
        .set_rst_n = set_rst_n,

        .get_addr = get_addr,
        .set_addr = set_addr,

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
