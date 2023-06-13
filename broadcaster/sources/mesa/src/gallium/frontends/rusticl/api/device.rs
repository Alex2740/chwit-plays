use crate::api::icd::*;
use crate::api::types::IdpAccelProps;
use crate::api::util::*;
use crate::core::device::*;
use crate::core::platform::*;
use crate::core::version::*;

use mesa_rust_gen::*;
use mesa_rust_util::ptr::*;
use rusticl_opencl_gen::*;
use rusticl_proc_macros::cl_entrypoint;
use rusticl_proc_macros::cl_info_entrypoint;

use std::cmp::min;
use std::ffi::CStr;
use std::mem::size_of;
use std::ptr;
use std::sync::Arc;

const SPIRV_SUPPORT_STRING: &str = "SPIR-V_1.0 SPIR-V_1.1 SPIR-V_1.2 SPIR-V_1.3 SPIR-V_1.4";
const SPIRV_SUPPORT: [cl_name_version; 5] = [
    mk_cl_version_ext(1, 0, 0, "SPIR-V"),
    mk_cl_version_ext(1, 1, 0, "SPIR-V"),
    mk_cl_version_ext(1, 2, 0, "SPIR-V"),
    mk_cl_version_ext(1, 3, 0, "SPIR-V"),
    mk_cl_version_ext(1, 4, 0, "SPIR-V"),
];
type ClDevIdpAccelProps = cl_device_integer_dot_product_acceleration_properties_khr;

#[cl_info_entrypoint(cl_get_device_info)]
impl CLInfo<cl_device_info> for cl_device_id {
    fn query(&self, q: cl_device_info, _: &[u8]) -> CLResult<Vec<u8>> {
        let dev = self.get_ref()?;

        // curses you CL_DEVICE_INTEGER_DOT_PRODUCT_ACCELERATION_PROPERTIES_4x8BIT_PACKED_KHR
        #[allow(non_upper_case_globals)]
        Ok(match q {
            CL_DEVICE_ADDRESS_BITS => cl_prop::<cl_uint>(dev.address_bits()),
            CL_DEVICE_ATOMIC_FENCE_CAPABILITIES => cl_prop::<cl_device_atomic_capabilities>(
                (CL_DEVICE_ATOMIC_ORDER_RELAXED
                    | CL_DEVICE_ATOMIC_ORDER_ACQ_REL
                    | CL_DEVICE_ATOMIC_SCOPE_WORK_GROUP)
                    as cl_device_atomic_capabilities,
            ),
            CL_DEVICE_ATOMIC_MEMORY_CAPABILITIES => cl_prop::<cl_device_atomic_capabilities>(
                (CL_DEVICE_ATOMIC_ORDER_RELAXED | CL_DEVICE_ATOMIC_SCOPE_WORK_GROUP)
                    as cl_device_atomic_capabilities,
            ),
            CL_DEVICE_AVAILABLE => cl_prop::<bool>(true),
            CL_DEVICE_BUILT_IN_KERNELS => cl_prop::<&str>(""),
            CL_DEVICE_BUILT_IN_KERNELS_WITH_VERSION => cl_prop::<Vec<cl_name_version>>(Vec::new()),
            CL_DEVICE_COMPILER_AVAILABLE => cl_prop::<bool>(true),
            CL_DEVICE_DEVICE_ENQUEUE_CAPABILITIES => {
                cl_prop::<cl_device_device_enqueue_capabilities>(0)
            }
            CL_DEVICE_DOUBLE_FP_CONFIG => cl_prop::<cl_device_fp_config>(
                if dev.doubles_supported() {
                    let mut fp64_config = CL_FP_FMA
                        | CL_FP_ROUND_TO_NEAREST
                        | CL_FP_ROUND_TO_ZERO
                        | CL_FP_ROUND_TO_INF
                        | CL_FP_INF_NAN
                        | CL_FP_DENORM;
                    if dev.doubles_is_softfp() {
                        fp64_config |= CL_FP_SOFT_FLOAT;
                    }
                    fp64_config
                } else {
                    0
                }
                .into(),
            ),
            CL_DEVICE_ENDIAN_LITTLE => cl_prop::<bool>(dev.little_endian()),
            CL_DEVICE_ERROR_CORRECTION_SUPPORT => cl_prop::<bool>(false),
            CL_DEVICE_EXECUTION_CAPABILITIES => {
                cl_prop::<cl_device_exec_capabilities>(CL_EXEC_KERNEL.into())
            }
            CL_DEVICE_EXTENSIONS => cl_prop::<&str>(&dev.extension_string),
            CL_DEVICE_EXTENSIONS_WITH_VERSION => cl_prop::<&Vec<cl_name_version>>(&dev.extensions),
            CL_DEVICE_GENERIC_ADDRESS_SPACE_SUPPORT => cl_prop::<bool>(false),
            CL_DEVICE_GLOBAL_MEM_CACHE_TYPE => cl_prop::<cl_device_mem_cache_type>(CL_NONE),
            CL_DEVICE_GLOBAL_MEM_CACHE_SIZE => cl_prop::<cl_ulong>(0),
            CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE => cl_prop::<cl_uint>(0),
            CL_DEVICE_GLOBAL_MEM_SIZE => cl_prop::<cl_ulong>(dev.global_mem_size()),
            CL_DEVICE_GLOBAL_VARIABLE_PREFERRED_TOTAL_SIZE => cl_prop::<usize>(0),
            CL_DEVICE_HALF_FP_CONFIG => cl_prop::<cl_device_fp_config>(0),
            CL_DEVICE_HOST_UNIFIED_MEMORY => cl_prop::<bool>(dev.unified_memory()),
            CL_DEVICE_IL_VERSION => cl_prop::<&str>(SPIRV_SUPPORT_STRING),
            CL_DEVICE_ILS_WITH_VERSION => cl_prop::<Vec<cl_name_version>>(SPIRV_SUPPORT.to_vec()),
            CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT => {
                cl_prop::<cl_uint>(dev.image_base_address_alignment())
            }
            CL_DEVICE_IMAGE_MAX_ARRAY_SIZE => cl_prop::<usize>(dev.image_array_size()),
            CL_DEVICE_IMAGE_MAX_BUFFER_SIZE => cl_prop::<usize>(dev.image_buffer_size()),
            CL_DEVICE_IMAGE_PITCH_ALIGNMENT => cl_prop::<cl_uint>(dev.image_pitch_alignment()),
            CL_DEVICE_IMAGE_SUPPORT => cl_prop::<bool>(dev.image_supported()),
            CL_DEVICE_IMAGE2D_MAX_HEIGHT => cl_prop::<usize>(dev.image_2d_size()),
            CL_DEVICE_IMAGE2D_MAX_WIDTH => cl_prop::<usize>(dev.image_2d_size()),
            CL_DEVICE_IMAGE3D_MAX_HEIGHT => cl_prop::<usize>(dev.image_3d_size()),
            CL_DEVICE_IMAGE3D_MAX_WIDTH => cl_prop::<usize>(dev.image_3d_size()),
            CL_DEVICE_IMAGE3D_MAX_DEPTH => cl_prop::<usize>(dev.image_3d_size()),
            CL_DEVICE_INTEGER_DOT_PRODUCT_CAPABILITIES_KHR => {
                cl_prop::<cl_device_integer_dot_product_capabilities_khr>(
                    (CL_DEVICE_INTEGER_DOT_PRODUCT_INPUT_4x8BIT_PACKED_KHR
                        | CL_DEVICE_INTEGER_DOT_PRODUCT_INPUT_4x8BIT_KHR)
                        .into(),
                )
            }
            CL_DEVICE_INTEGER_DOT_PRODUCT_ACCELERATION_PROPERTIES_8BIT_KHR => {
                cl_prop::<ClDevIdpAccelProps>({
                    let pack = dev.pack_32_4x8_supported();
                    let sdot = dev.sdot_4x8_supported() && pack;
                    let udot = dev.udot_4x8_supported() && pack;
                    let sudot = dev.sudot_4x8_supported() && pack;
                    IdpAccelProps::new(
                        sdot.into(),
                        udot.into(),
                        sudot.into(),
                        sdot.into(),
                        udot.into(),
                        sudot.into(),
                    )
                })
            }
            CL_DEVICE_INTEGER_DOT_PRODUCT_ACCELERATION_PROPERTIES_4x8BIT_PACKED_KHR => {
                cl_prop::<ClDevIdpAccelProps>({
                    IdpAccelProps::new(
                        dev.sdot_4x8_supported().into(),
                        dev.udot_4x8_supported().into(),
                        dev.sudot_4x8_supported().into(),
                        dev.sdot_4x8_supported().into(),
                        dev.udot_4x8_supported().into(),
                        dev.sudot_4x8_supported().into(),
                    )
                })
            }

            CL_DEVICE_LATEST_CONFORMANCE_VERSION_PASSED => {
                cl_prop::<&CStr>(dev.screen().cl_cts_version())
            }
            CL_DEVICE_LINKER_AVAILABLE => cl_prop::<bool>(true),
            CL_DEVICE_LOCAL_MEM_SIZE => cl_prop::<cl_ulong>(dev.local_mem_size()),
            // TODO add query for CL_LOCAL vs CL_GLOBAL
            CL_DEVICE_LOCAL_MEM_TYPE => cl_prop::<cl_device_local_mem_type>(CL_GLOBAL),
            CL_DEVICE_LUID_KHR => cl_prop::<[cl_uchar; CL_LUID_SIZE_KHR as usize]>(
                dev.screen().device_luid().unwrap_or_default(),
            ),
            CL_DEVICE_LUID_VALID_KHR => {
                cl_prop::<cl_bool>(dev.screen().device_luid().is_some().into())
            }
            CL_DEVICE_MAX_CLOCK_FREQUENCY => cl_prop::<cl_uint>(dev.max_clock_freq()),
            CL_DEVICE_MAX_COMPUTE_UNITS => cl_prop::<cl_uint>(dev.max_compute_units()),
            // TODO atm implemented as mem_const
            CL_DEVICE_MAX_CONSTANT_ARGS => cl_prop::<cl_uint>(dev.const_max_count()),
            CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE => cl_prop::<cl_ulong>(dev.const_max_size()),
            CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE => cl_prop::<usize>(0),
            CL_DEVICE_MAX_MEM_ALLOC_SIZE => cl_prop::<cl_ulong>(dev.max_mem_alloc()),
            CL_DEVICE_MAX_NUM_SUB_GROUPS => cl_prop::<cl_uint>(0),
            CL_DEVICE_MAX_ON_DEVICE_EVENTS => cl_prop::<cl_uint>(0),
            CL_DEVICE_MAX_ON_DEVICE_QUEUES => cl_prop::<cl_uint>(0),
            CL_DEVICE_MAX_PARAMETER_SIZE => cl_prop::<usize>(dev.param_max_size()),
            CL_DEVICE_MAX_PIPE_ARGS => cl_prop::<cl_uint>(0),
            CL_DEVICE_MAX_READ_IMAGE_ARGS => cl_prop::<cl_uint>(dev.image_read_count()),
            CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS => {
                cl_prop::<cl_uint>(if dev.image_read_write_supported() {
                    dev.image_write_count()
                } else {
                    0
                })
            }
            CL_DEVICE_MAX_SAMPLERS => cl_prop::<cl_uint>(dev.max_samplers()),
            CL_DEVICE_MAX_WORK_GROUP_SIZE => cl_prop::<usize>(dev.max_threads_per_block()),
            CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS => cl_prop::<cl_uint>(dev.max_grid_dimensions()),
            CL_DEVICE_MAX_WORK_ITEM_SIZES => cl_prop::<Vec<usize>>(dev.max_block_sizes()),
            CL_DEVICE_MAX_WRITE_IMAGE_ARGS => cl_prop::<cl_uint>(dev.image_write_count()),
            // TODO proper retrival from devices
            CL_DEVICE_MEM_BASE_ADDR_ALIGN => cl_prop::<cl_uint>(0x1000),
            CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE => {
                cl_prop::<cl_uint>(size_of::<cl_ulong16>() as cl_uint)
            }
            CL_DEVICE_NAME => cl_prop(dev.screen().name()),
            CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR => cl_prop::<cl_uint>(1),
            CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE => {
                cl_prop::<cl_uint>(if dev.doubles_supported() { 1 } else { 0 })
            }
            CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT => cl_prop::<cl_uint>(1),
            CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF => cl_prop::<cl_uint>(0),
            CL_DEVICE_NATIVE_VECTOR_WIDTH_INT => cl_prop::<cl_uint>(1),
            CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG => cl_prop::<cl_uint>(1),
            CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT => cl_prop::<cl_uint>(1),
            CL_DEVICE_NODE_MASK_KHR => {
                cl_prop::<cl_uint>(dev.screen().device_node_mask().unwrap_or_default())
            }
            CL_DEVICE_NON_UNIFORM_WORK_GROUP_SUPPORT => cl_prop::<bool>(false),
            CL_DEVICE_NUMERIC_VERSION => cl_prop::<cl_version>(dev.cl_version as cl_version),
            // TODO subdevice support
            CL_DEVICE_PARENT_DEVICE => cl_prop::<cl_device_id>(cl_device_id::from_ptr(ptr::null())),
            CL_DEVICE_PARTITION_AFFINITY_DOMAIN => cl_prop::<cl_device_affinity_domain>(0),
            CL_DEVICE_PARTITION_MAX_SUB_DEVICES => cl_prop::<cl_uint>(0),
            CL_DEVICE_PARTITION_PROPERTIES => cl_prop::<Vec<cl_device_partition_property>>(vec![0]),
            CL_DEVICE_PARTITION_TYPE => cl_prop::<Vec<cl_device_partition_property>>(Vec::new()),
            CL_DEVICE_PCI_BUS_INFO_KHR => {
                cl_prop::<cl_device_pci_bus_info_khr>(dev.pci_info().ok_or(CL_INVALID_VALUE)?)
            }
            CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS => cl_prop::<cl_uint>(0),
            CL_DEVICE_PIPE_MAX_PACKET_SIZE => cl_prop::<cl_uint>(0),
            CL_DEVICE_PIPE_SUPPORT => cl_prop::<bool>(false),
            CL_DEVICE_PLATFORM => cl_prop::<cl_platform_id>(Platform::get().as_ptr()),
            CL_DEVICE_PREFERRED_GLOBAL_ATOMIC_ALIGNMENT => cl_prop::<cl_uint>(0),
            CL_DEVICE_PREFERRED_INTEROP_USER_SYNC => cl_prop::<bool>(true),
            CL_DEVICE_PREFERRED_LOCAL_ATOMIC_ALIGNMENT => cl_prop::<cl_uint>(0),
            CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT => cl_prop::<cl_uint>(0),
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR => cl_prop::<cl_uint>(1),
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE => {
                cl_prop::<cl_uint>(if dev.doubles_supported() { 1 } else { 0 })
            }
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT => cl_prop::<cl_uint>(1),
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF => cl_prop::<cl_uint>(0),
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT => cl_prop::<cl_uint>(1),
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG => cl_prop::<cl_uint>(1),
            CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT => cl_prop::<cl_uint>(1),
            CL_DEVICE_PREFERRED_WORK_GROUP_SIZE_MULTIPLE => {
                cl_prop::<usize>(dev.subgroups() as usize)
            }
            CL_DEVICE_PRINTF_BUFFER_SIZE => cl_prop::<usize>(dev.printf_buffer_size()),
            // TODO
            CL_DEVICE_PROFILING_TIMER_RESOLUTION => cl_prop::<usize>(0),
            CL_DEVICE_OPENCL_C_FEATURES => cl_prop::<&Vec<cl_name_version>>(&dev.clc_features),
            CL_DEVICE_OPENCL_C_VERSION => {
                cl_prop::<String>(format!("OpenCL C {} ", dev.clc_version.api_str()))
            }
            CL_DEVICE_OPENCL_C_ALL_VERSIONS => cl_prop::<&Vec<cl_name_version>>(&dev.clc_versions),
            CL_DEVICE_PROFILE => cl_prop(if dev.embedded {
                "EMBEDDED_PROFILE"
            } else {
                "FULL_PROFILE"
            }),
            CL_DEVICE_QUEUE_ON_DEVICE_MAX_SIZE => cl_prop::<cl_uint>(0),
            CL_DEVICE_QUEUE_ON_DEVICE_PREFERRED_SIZE => cl_prop::<cl_uint>(0),
            CL_DEVICE_QUEUE_ON_DEVICE_PROPERTIES => cl_prop::<cl_command_queue_properties>(0),
            CL_DEVICE_QUEUE_ON_HOST_PROPERTIES => {
                cl_prop::<cl_command_queue_properties>(CL_QUEUE_PROFILING_ENABLE.into())
            }
            CL_DEVICE_REFERENCE_COUNT => cl_prop::<cl_uint>(1),
            CL_DEVICE_SINGLE_FP_CONFIG => cl_prop::<cl_device_fp_config>(
                (CL_FP_ROUND_TO_NEAREST | CL_FP_INF_NAN) as cl_device_fp_config,
            ),
            CL_DEVICE_SUB_GROUP_INDEPENDENT_FORWARD_PROGRESS => cl_prop::<bool>(false),
            CL_DEVICE_SVM_CAPABILITIES | CL_DEVICE_SVM_CAPABILITIES_ARM => {
                cl_prop::<cl_device_svm_capabilities>(
                    if dev.svm_supported() {
                        CL_DEVICE_SVM_COARSE_GRAIN_BUFFER
                            | CL_DEVICE_SVM_FINE_GRAIN_BUFFER
                            | CL_DEVICE_SVM_FINE_GRAIN_SYSTEM
                    } else {
                        0
                    }
                    .into(),
                )
            }
            CL_DEVICE_TYPE => cl_prop::<cl_device_type>(dev.device_type(false)),
            CL_DEVICE_UUID_KHR => cl_prop::<[cl_uchar; CL_UUID_SIZE_KHR as usize]>(
                dev.screen().device_uuid().unwrap_or_default(),
            ),
            CL_DEVICE_VENDOR => cl_prop(dev.screen().device_vendor()),
            CL_DEVICE_VENDOR_ID => cl_prop::<cl_uint>(dev.vendor_id()),
            CL_DEVICE_VERSION => cl_prop::<String>(format!("OpenCL {} ", dev.cl_version.api_str())),
            CL_DRIVER_UUID_KHR => cl_prop::<[cl_char; CL_UUID_SIZE_KHR as usize]>(
                dev.screen().driver_uuid().unwrap_or_default(),
            ),
            CL_DRIVER_VERSION => cl_prop::<&CStr>(unsafe { CStr::from_ptr(mesa_version_string()) }),
            CL_DEVICE_WORK_GROUP_COLLECTIVE_FUNCTIONS_SUPPORT => cl_prop::<bool>(false),
            // CL_INVALID_VALUE if param_name is not one of the supported values
            // CL_INVALID_VALUE [...] if param_name is a value that is available as an extension and the corresponding extension is not supported by the device.
            _ => return Err(CL_INVALID_VALUE),
        })
    }
}

fn devs() -> &'static Vec<Arc<Device>> {
    &Platform::get().devs
}

pub fn get_devs_for_type(device_type: cl_device_type) -> Vec<&'static Arc<Device>> {
    devs()
        .iter()
        .filter(|d| device_type & d.device_type(true) != 0)
        .collect()
}

#[cl_entrypoint]
fn get_device_ids(
    platform: cl_platform_id,
    device_type: cl_device_type,
    num_entries: cl_uint,
    devices: *mut cl_device_id,
    num_devices: *mut cl_uint,
) -> CLResult<()> {
    // CL_INVALID_PLATFORM if platform is not a valid platform.
    platform.get_ref()?;

    // CL_INVALID_DEVICE_TYPE if device_type is not a valid value.
    check_cl_device_type(device_type)?;

    // CL_INVALID_VALUE if num_entries is equal to zero and devices is not NULL
    if num_entries == 0 && !devices.is_null() {
        return Err(CL_INVALID_VALUE);
    }

    // CL_INVALID_VALUE [...] if both num_devices and devices are NULL.
    if num_devices.is_null() && devices.is_null() {
        return Err(CL_INVALID_VALUE);
    }

    let devs = get_devs_for_type(device_type);
    // CL_DEVICE_NOT_FOUND if no OpenCL devices that matched device_type were found
    if devs.is_empty() {
        return Err(CL_DEVICE_NOT_FOUND);
    }

    // num_devices returns the number of OpenCL devices available that match device_type. If
    // num_devices is NULL, this argument is ignored.
    num_devices.write_checked(devs.len() as cl_uint);

    if !devices.is_null() {
        let n = min(num_entries as usize, devs.len());

        #[allow(clippy::needless_range_loop)]
        for i in 0..n {
            unsafe {
                // Note we use as_ptr here which doesn't increase the reference count.
                *devices.add(i) = cl_device_id::from_ptr(Arc::as_ptr(devs[i]));
            }
        }
    }

    Ok(())
}

#[cl_entrypoint]
fn retain_device(_device: cl_device_id) -> CLResult<()> {
    Ok(())
}

#[cl_entrypoint]
fn release_device(_device: cl_device_id) -> CLResult<()> {
    Ok(())
}

#[cl_entrypoint]
fn get_device_and_host_timer(
    _device: cl_device_id,
    _device_timestamp: *mut cl_ulong,
    _host_timestamp: *mut cl_ulong,
) -> CLResult<()> {
    // TODO: we could support it
    Err(CL_INVALID_OPERATION)
}

#[cl_entrypoint]
fn get_host_timer(_device: cl_device_id, _host_timestamp: *mut cl_ulong) -> CLResult<()> {
    // TODO: we could support it
    Err(CL_INVALID_OPERATION)
}

#[cl_entrypoint]
fn set_default_device_command_queue(
    _context: cl_context,
    _device: cl_device_id,
    _command_queue: cl_command_queue,
) -> CLResult<()> {
    Err(CL_INVALID_OPERATION)
}
