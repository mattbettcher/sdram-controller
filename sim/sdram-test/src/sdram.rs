#![allow(dead_code)]

#[repr(C)]
pub struct Env {
    get_reset_n: extern "C" fn() -> u32,
    set_reset_n: extern "C" fn(value: u32),

    get_clk: extern "C" fn() -> u32,
    set_clk: extern "C" fn(value: u32),

    get_inc: extern "C" fn() -> u32,
    set_inc: extern "C" fn(value: u32),

    get_count: extern "C" fn() -> u32,
    set_count: extern "C" fn(value: u32),

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
        unsafe { ((*self.env).get_reset_n)() != 0 }
    }

    pub fn set_reset_n(&mut self, value: bool) {
        unsafe {
            ((*self.env).set_reset_n)(if value { 1 } else { 0 });
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

    pub fn inc(&self) -> bool {
        unsafe { ((*self.env).get_inc)() != 0 }
    }

    pub fn set_inc(&mut self, value: bool) {
        unsafe {
            ((*self.env).set_inc)(if value { 1 } else { 0 });
        }
    }

    pub fn count(&self) -> u32 {
        unsafe { ((*self.env).get_count)() }
    }

    pub fn set_count(&mut self, value: u32) {
        unsafe {
            ((*self.env).set_count)(value);
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