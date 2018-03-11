#![allow(dead_code)]

#[repr(C)]
pub struct Env {
    
    get_clk: extern "C" fn() -> u32,
    set_clk: extern "C" fn(value: u32),

    get_ba: extern "C" fn() -> u32,
    set_ba: extern "C" fn(value: u32),

    get_cas_n: extern "C" fn() -> u32,
    set_cas_n: extern "C" fn(value: u32),

    get_cs_n: extern "C" fn() -> u32,
    set_cs_n: extern "C" fn(value: u32),

    get_dqmh: extern "C" fn() -> u32,
    set_dqmh: extern "C" fn(value: u32),

    get_dqml: extern "C" fn() -> u32,
    set_dqml: extern "C" fn(value: u32),

    get_ras_n: extern "C" fn() -> u32,
    set_ras_n: extern "C" fn(value: u32),

    get_we_n: extern "C" fn() -> u32,
    set_we_n: extern "C" fn(value: u32),
    
    get_rst_n: extern "C" fn() -> u32,
    set_rst_n: extern "C" fn(value: u32),

    get_addr: extern "C" fn() -> u32,
    set_addr: extern "C" fn(value: u32),

    eval: extern "C" fn(),
    final_: extern "C" fn(),
    trace_dump: extern "C" fn(time: u64),
}

pub struct Sdram {
    env: *const Env,
}

impl Sdram {
    pub fn new(env: *const Env) -> Sdram {
        Sdram {
            env: env,
        }
    }

    pub fn reset_n(&self) -> bool {
        unsafe { ((*self.env).get_rst_n)() != 0 }
    }

    pub fn set_reset_n(&mut self, value: bool) {
        unsafe {
            ((*self.env).set_rst_n)(if value { 1 } else { 0 });
        }
    }

    pub fn clk(&self) -> bool {
        unsafe { ((*self.env).get_clk)() != 0 }
    }

    pub fn set_clk(&mut self, value: bool) {
        unsafe {
            ((*self.env).set_clk)(if value { 1 } else { 0 });
        }
    }

    pub fn eval(&mut self) {
        unsafe {
            ((*self.env).eval)();
        }
    }

    pub fn final_(&mut self) {
        unsafe {
            ((*self.env).final_)();
        }
    }

    pub fn trace_dump(&mut self, time: u64) {
        unsafe {
            ((*self.env).trace_dump)(time);
        }
    }
}