/*
 * Copyright (c) 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#include "precompiled.hpp"
#include "gc_implementation/parallelScavenge/heterogeneousGenerationSizer.hpp"
#include "memory/collectorPolicy.hpp"
#include "runtime/globals_extension.hpp"
#include "runtime/os.hpp"
#include "utilities/align.hpp"
#include "utilities/globalDefinitions.hpp"

const double HeterogeneousGenerationSizer::MaxRamFractionForYoung = 0.8;

// Check the available dram memory to limit NewSize and MaxNewSize before
// calling base class initialize_flags().
void HeterogeneousGenerationSizer::initialize_flags() {
  FormatBuffer<100> calc_str("In ");

  julong phys_mem;
  // If MaxRam is specified, we use that as maximum physical memory available.
  if (FLAG_IS_DEFAULT(MaxRAM)) {
    phys_mem = os::physical_memory();
    calc_str.append("Physical_Memory");
  } else {
    phys_mem = (julong)MaxRAM;
    calc_str.append("MaxRAM");
  }

  julong reasonable_max = phys_mem;

  // If either MaxRAMFraction or MaxRAMPercentage is specified, we use them to calculate
  // reasonable max size of young generation.
  if (!FLAG_IS_DEFAULT(MaxRAMFraction)) {
    reasonable_max = (julong)(phys_mem / MaxRAMFraction);
    calc_str.append(" / MaxRAMFraction");
  } else if (!FLAG_IS_DEFAULT(MaxRAMPercentage)) {
    reasonable_max = (julong)((phys_mem * MaxRAMPercentage) / 100);
    calc_str.append(" * MaxRAMPercentage / 100");
  } else {
    // We use our own fraction to calculate max size of young generation.
    reasonable_max = phys_mem * MaxRamFractionForYoung;
    calc_str.append(" * %0.2f", MaxRamFractionForYoung);
  }
  reasonable_max = align_up(reasonable_max, _gen_alignment);

  if (MaxNewSize > reasonable_max) {
    if (FLAG_IS_CMDLINE(MaxNewSize)) {
      debug_only(fprintf(stderr, "Setting MaxNewSize to " SIZE_FORMAT " based on dram available (calculation = align(%s))",
                            (size_t)reasonable_max, calc_str.buffer()));
    } else {
      debug_only(fprintf(stderr, "Setting MaxNewSize to " SIZE_FORMAT " based on dram available (calculation = align(%s)). "
                         "Dram usage can be lowered by setting MaxNewSize to a lower value", (size_t)reasonable_max, calc_str.buffer()));
    }
    MaxNewSize = reasonable_max;
  }
  if (NewSize > reasonable_max) {
    if (FLAG_IS_CMDLINE(NewSize)) {
      debug_only(fprintf(stderr, "Setting NewSize to " SIZE_FORMAT " based on dram available (calculation = align(%s))",
                            (size_t)reasonable_max, calc_str.buffer()));
    }
    NewSize = reasonable_max;
  }

  // After setting new size flags, call base class initialize_flags()
  GenerationSizer::initialize_flags();
}

bool HeterogeneousGenerationSizer::is_hetero_heap() const {
  return true;
}

size_t HeterogeneousGenerationSizer::heap_reserved_size_bytes() const {
  if (UseAdaptiveGCBoundary) {
    // This is the size that young gen can grow to, when UseAdaptiveGCBoundary is true.
    size_t max_yg_size = _max_heap_byte_size - _min_gen1_size;
    // This is the size that old gen can grow to, when UseAdaptiveGCBoundary is true.
    size_t max_old_size = _max_heap_byte_size - _min_gen0_size;

    return max_yg_size + max_old_size;
  } else {
    return _max_heap_byte_size;
  }
}
