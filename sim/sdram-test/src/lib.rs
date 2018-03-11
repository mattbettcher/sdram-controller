mod sdram;

use sdram::*;

#[no_mangle]
pub extern "C" fn run(env: *const Env) -> i32 {
    let mut test = Sdram::new(env);
    let mut time = 0;
        
    // Reset
    test.set_reset_n(false);
    test.set_clk(false);
    
    trace(&mut test, &mut time);

    test.set_clk(true);

    trace(&mut test, &mut time);

    test.set_reset_n(true);
    test.set_clk(false);

    trace(&mut test, &mut time);

    for _ in 0..600 {
        // Rising edge
        test.set_clk(true);

        // Do other stuff here!
        

        trace(&mut test, &mut time);

        // Falling edge
        test.set_clk(false);
        trace(&mut test, &mut time);
    }

    test.final_();

    0
}

fn trace(test: &mut Sdram, time: &mut u64) {
    test.eval();
    test.trace_dump(*time);
    *time += 1; 
}