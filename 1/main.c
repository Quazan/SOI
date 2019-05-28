int do_getprocnr(void)
{
 int procnr;

 for(procnr = 0; procnr < NR_PROCS; ++procnr)
 {
  if(((mproc[procnr].mp_flags & IN_USE) != 0) && (mproc[procnr].mp_pid == pid))
  {
   return procnr;
  }
 }
 return ENOENT;
}
