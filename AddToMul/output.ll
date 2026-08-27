; ModuleID = 'test.ll'
source_filename = "test.ll"

define i32 @test(i32 %a, i32 %b) {
entry:
  %x_mul = mul i32 %a, %b
  %y_mul = mul i32 %x_mul, 2
  ret i32 %y_mul
}
