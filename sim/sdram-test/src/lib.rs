mod sdram;

use sdram::*;

static mut TIME: u64 = 0;

#[no_mangle]
pub extern "C" fn run(env: *const Env) -> i32 {
    let mut test = Sdram::new(env);
        
    // Reset
    test.set_reset_n(false);
    test.set_clk(false);
    test.set_inc(false);
    test.set_count(0);
    
    trace(&mut test);

    test.set_clk(true);

    trace(&mut test);

    test.set_reset_n(true);
    test.set_clk(false);

    trace(&mut test);

    let inc_off = 50;

    for _ in 0..600 {
        // Rising edge
        test.set_clk(true);
        test.set_inc(true);

        // Do other stuff here!
        unsafe {
            if TIME >= inc_off {
                test.set_inc(false);
            }
        }

        trace(&mut test);

        // Falling edge
        test.set_clk(false);
        trace(&mut test);
    }

    test.final_();

    0
}

fn trace(test: &mut Sdram) {
    test.eval();
    unsafe { 
        test.trace_dump(TIME);
        TIME += 1; 
    }
}