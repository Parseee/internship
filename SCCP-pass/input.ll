; ModuleID = 'tests/input.c'
source_filename = "tests/input.c"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @req1() #0 {
  %1 = alloca i32, align 4
  %2 = load i32, i32* %1, align 4
  %3 = mul nsw i32 %2, 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @req2() #0 {
  %1 = alloca i32, align 4
  store i32 1, i32* %1, align 4
  store i32 2, i32* %1, align 4
  store i32 3, i32* %1, align 4
  store i32 4, i32* %1, align 4
  store i32 5, i32* %1, align 4
  %2 = load i32, i32* %1, align 4
  %3 = mul nsw i32 %2, 4
  ret i32 %3
}

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="non-leaf" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Debian clang version 11.0.1-2"}
