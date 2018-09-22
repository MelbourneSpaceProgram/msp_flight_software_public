# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: LocationReading.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='LocationReading.proto',
  package='',
  syntax='proto2',
  serialized_pb=_b('\n\x15LocationReading.proto\"\x9a\x01\n\x0fLocationReading\x12\"\n\x1alattitude_geodetic_degrees\x18\x01 \x02(\x01\x12\x19\n\x11longitude_degrees\x18\x02 \x02(\x01\x12#\n\x1b\x61ltitude_above_ellipsoid_km\x18\x03 \x02(\x01\x12#\n\x1btimestamp_ms\x18\x04 \x02(\x06')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_LOCATIONREADING = _descriptor.Descriptor(
  name='LocationReading',
  full_name='LocationReading',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='lattitude_geodetic_degrees', full_name='LocationReading.lattitude_geodetic_degrees', index=0,
      number=1, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='longitude_degrees', full_name='LocationReading.longitude_degrees', index=1,
      number=2, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='altitude_above_ellipsoid_km', full_name='LocationReading.altitude_above_ellipsoid_km', index=2,
      number=3, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='timestamp_ms', full_name='LocationReading.timestamp_ms', index=3,
      number=4, type=6, cpp_type=4, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=26,
  serialized_end=180,
)

DESCRIPTOR.message_types_by_name['LocationReading'] = _LOCATIONREADING

LocationReading = _reflection.GeneratedProtocolMessageType('LocationReading', (_message.Message,), dict(
  DESCRIPTOR = _LOCATIONREADING,
  __module__ = 'LocationReading_pb2'
  # @@protoc_insertion_point(class_scope:LocationReading)
  ))
_sym_db.RegisterMessage(LocationReading)


# @@protoc_insertion_point(module_scope)
