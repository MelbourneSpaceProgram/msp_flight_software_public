# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: GyrometerReading.proto

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
  name='GyrometerReading.proto',
  package='',
  syntax='proto2',
  serialized_pb=_b('\n\x16GyrometerReading.proto\"X\n\x10GyrometerReading\x12\t\n\x01x\x18\x01 \x02(\x01\x12\t\n\x01y\x18\x02 \x02(\x01\x12\t\n\x01z\x18\x03 \x02(\x01\x12#\n\x1btimestamp_millis_unix_epoch\x18\x04 \x02(\x06')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_GYROMETERREADING = _descriptor.Descriptor(
  name='GyrometerReading',
  full_name='GyrometerReading',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='x', full_name='GyrometerReading.x', index=0,
      number=1, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='y', full_name='GyrometerReading.y', index=1,
      number=2, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='z', full_name='GyrometerReading.z', index=2,
      number=3, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='timestamp_millis_unix_epoch', full_name='GyrometerReading.timestamp_millis_unix_epoch', index=3,
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
  serialized_end=114,
)

DESCRIPTOR.message_types_by_name['GyrometerReading'] = _GYROMETERREADING

GyrometerReading = _reflection.GeneratedProtocolMessageType('GyrometerReading', (_message.Message,), dict(
  DESCRIPTOR = _GYROMETERREADING,
  __module__ = 'GyrometerReading_pb2'
  # @@protoc_insertion_point(class_scope:GyrometerReading)
  ))
_sym_db.RegisterMessage(GyrometerReading)


# @@protoc_insertion_point(module_scope)