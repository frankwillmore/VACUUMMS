# ROADMAP

---

Features planned and potential releases:

- Python integration:
  - Create package vacuumms with subpackages for utils, applications, and libraries? 
  - Could just wrap shell commands with python functions and capture their output.
  - Need examples, of course.

- HDF5 file support:
  - The existing formats are simple lists and tab-separated values, which could be easily represented in HDF5
  - Could be as simple as writing a translator for each of the file types used
  - Might best be handled by presenting as python interface

- Limits:
  - Use of limits.h more broadly; there's no reason to be arbitrarily limiting size of data structures based on sizes that were considered 'reasonable' ten years ago.
  - Replace static 
  - Need careful reworking of original int types to long. Maybe even declare a vacuumms_types.h? 
  - Replace the hard-coded types used for storing configurations and Energy arrays with C++ Template classes 

- Configurable include files, where sizes can be specified at configure time.
  - vacuumms.h header that includes version numbers, etc. and which is updated when cutting a release.

- Generalize to pluggable force field models.

- Tests:
  - Add tests for verifying integrity of builds. CTest?

(24 Mar 2024)
- Runtime library and API
  - Beginning with VACUUMMS/1.2.0, and now with additional header files, the include path <vacuumms/*> can be considered a user-facing API to the new VACUUMMS C++ runtime library, libvacuumms_cpp.so. The variational module already makes use of this API for it's user-facing, user-buildable examples.
  - Some existing VACUUMMS code will be factored to this and/or a new C runtime library, eventually replacing the vestigially named libftw_general.so, etc. 


