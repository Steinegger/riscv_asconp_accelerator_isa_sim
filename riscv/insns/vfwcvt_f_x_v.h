// vfwcvt.f.x.v vd, vs2, vm
VI_VFP_LOOP_BASE
  VI_CHECK_DSS(false);
  auto vs2 = P.VU.elt<int32_t>(rs2_num, i);
  P.VU.elt<float64_t>(rd_num, i) = i32_to_f64(vs2);
  set_fp_exceptions;
VI_VFP_LOOP_WIDE_END
