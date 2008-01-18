interface

const
  HASH_TTH = 0;
  HASH_SHA1 = 1;
  HASH_AICH = 2;

function hash_create(type: Integer; size: Int64): LongWord cdecl; external 'hash.dll' name 'hash_create';
function hash_update(id: LongWord; buf: PChar; len: Integer): Integer cdecl; external 'hash.dll' name 'hash_update';
function hash_digest(id: LongWord; sum: PChar; len: Integer): Integer cdecl; external 'hash.dll' name 'hash_digest';

implementation

end.